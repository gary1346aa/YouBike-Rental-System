//
//  main.cpp
//  DS Final Project
//
//  Created by DSC103000002 on 5/27/15.
//  Copyright (c) 2015 Apple. All rights reserved.
//
//
#include <algorithm>
#include <iostream>
#include <cstring>
#include <fstream>
#include <cmath>
#include <iomanip>
#include <vector>
using namespace std;
ofstream fileOut;
ifstream testCaseIn;
ifstream mapIn;
//Data Types
enum StatusType {Free, Rented};
enum ClassType  {Electric, Lady, Road, Hybrid};
enum StationType
{
    Danshui=0, Hongshulin=1, Beitou=2,
    Shilin=3, Zhongshan=4, Xinpu=5,
    Ximen=6, Liuzhangli=7, Muzha=8,
    Guting=9, Gongguan=10, Jingmei=11
};
typedef char LicenseType[6];
struct BikeType
{
    LicenseType License;
    StatusType Status;
    int Mileage=0; /* most recently rented mileage */
    int Cursor=0; /* cursor to the entry in heap where there is a pointer to this node */
    StationType Station;
    ClassType Class;
};
typedef struct BikeType *BikePtr;
struct HeapType
{
    BikePtr Elem[256]= {}; /* use array to implement heap, and each node in the heap is a pointer*/
    int Number=0;
};
struct Stationtype
{
    int Net=0; /* total income of station */
    int NetElectric=0;
    int NetLady=0;
    int NetRoad=0;
    int NetHybrid=0;
    int NumElectric=0; /* number of electric bikes */
    int NumLady=0; /* number of lady bikes */
    int NumRoad=0; /* number of road bikes */
    int NumHybrid=0; /* number of hybrid bikes */
    HeapType HRent;
    HeapType HElectric;
    HeapType HLady;
    HeapType HRoad;
    HeapType HHybrid;
};
struct HNodeType
{
    LicenseType License ;
    BikePtr Ptr; /* point to the node representing the bike */
    HNodeType *next_ptr=0; /*point to the next node in the chaining list*/
};
struct HTableType
{
    HNodeType table[256];
    /*since each entry in the hash table is a pointer, it will be initialized as NULL;*/
    int BikeNum; /* the number of bikes in the hash table */
};
class Graph
{
public:
    void ShortestPath(const int, const int);
    int choose(const int);
    int length[12][12];
    int a[12][12];
    int dist[12];
    bool s[12];
};
//

//functions
BikePtr     NewBike(LicenseType L,int Mile, ClassType Class, StationType Station);
BikePtr     SearchBike(LicenseType L);
int         JunkBikePtr(BikePtr Bike);
void        TransBikePtr(StationType Station, BikePtr Bike);
void        RentBikePtr(StationType s, BikePtr Bike);
int         Returns(StationType s,BikePtr Bike, int ReturnMile);
void        Inquire(LicenseType License);
void        StationReport(StationType s);
void        UbikeReport();
void        NetSearch(StationType s);
void        HashReport();
void        Sortheap(HeapType& Heap, int num, int mode);
void        hashnew(LicenseType L,BikePtr Bike, int num,int mode);
StationType stoST(string s="",int a=12);
ClassType   stoClassType(string in);
int         inttoLicenseType(LicenseType in);
HeapType&   Dealwithclass(ClassType c,StationType s);
string      Dealwithstation(StationType s);
int&        DealwithNum (StationType s,ClassType c);
int         Dealwithcharge(ClassType c,StationType start,StationType end,int Mile);
int         Printfree (StationType s, int a,bool st);
int         Printrented (StationType s,int a,bool st);
//

//global variables:
Stationtype Allstation[12];  //Store station data
HTableType  Hash;            //Another system to store bike
Graph Map;                   //Store Original Map data and apply Dijkstra's algorithm
//

//Implementation
int main(int argc, const char * argv[])
{
    /* if(argc!=4){
         fileOut<< "arguments are incorrect"<<endl;
         return 0;
     }*/

//fileOut.open(argv[3],ifstream::out);
    mapIn.open("testMap",ifstream::in);
//testCaseIn.open(argv[1],ifstream::in);

    string s, t;
    int dist;
    for(int i=0; i<12; i++)
        for(int j=0; j<12; j++)
            if(i!=j)Map.length[i][j]=10000;
    while(mapIn >> s >> t >> dist)
    {
        StationType SStation,EStation;
        SStation=stoST(s);
        EStation=stoST(t);
        Map.length[SStation][EStation]= dist;
        Map.length[EStation][SStation]= dist;
    }
    for(int i=0; i<12; i++)
    {
        Map.ShortestPath(12,i);
        for(int j=0; j<12; j++)
            Map.a[i][j]=Map.dist[j];
    }
    string cmd;
    while(cin >> cmd)
    {
        if(cmd == "NewBike")
        {
            string x, z;
            int Mile;
            LicenseType License;
            StationType Station;
            ClassType Class;
            cin >> x >> License >> Mile >> z;
            Station=stoST(z);
            Class=stoClassType(x);
            BikePtr Bike = NewBike(License, Mile, Class, Station);
            x=Dealwithstation(Bike->Station);
            cout<<"New bike is received by Station "<<z<<"."<<endl;
        }
        else if(cmd == "JunkIt")
        {
            LicenseType License;
            StationType S;
            cin>>License;
            BikePtr Bike = SearchBike(License);
            if(Bike) S = Bike->Station;
            int test = JunkBikePtr(Bike);
            if(test==2)
            {
                cout<<"Bike "<<License<<" does not belong to our company."<<endl;
            }
            else if(test==1)
            {
                cout<<"Bike "<<License<<" is now being rented."<<endl;
            }
            else if(test==0)
            {
                cout<<"Bike "<<License<<" is deleted from ";
                string x=Dealwithstation(S);
                cout<<x<<"."<<endl;
            }
        }
        else if (cmd == "Trans")
        {
            string x;
            LicenseType License;
            StationType Station;
            cin >> x >> License;
            Station = stoST(x);
            BikePtr Bike = SearchBike(License);
            if(!Bike)
            {
                cout<<"Bike "<<License<<" does not belong to our company."<<endl;
            }
            else TransBikePtr(Station, Bike);
        }
        else if (cmd == "Rent")
        {
            string x, y;
            StationType Station;
            ClassType Class;
            cin >> x >> y;
            Station = stoST(x);
            Class = stoClassType(y);
            BikePtr ptr = Dealwithclass(Class,Station).Elem[0];
            if(ptr)
            {
                RentBikePtr(Station,ptr);
                cout << "A bike is rented from " << Dealwithstation(Station) <<"."<< endl;
            }
            else cout << "No free bike is available." << endl;
        }
        else if(cmd == "Returns")
        {
            string x;
            int Mile;
            LicenseType License;
            StationType Station;
            int charge;
            cin >> x >> License >> Mile;
            Station = stoST(x);
            BikePtr Bike = SearchBike(License);
            charge = Returns(Station,Bike,Mile);
            Allstation[Bike->Station].Net+=charge;
            switch(Bike->Class)
            {
            case(Electric):
            {
                Allstation[Bike->Station].NetElectric+=charge;
                break;
            }
            case(Lady):
            {
                Allstation[Bike->Station].NetLady+=charge;
                break;
            }
            case(Road):
            {
                Allstation[Bike->Station].NetRoad+=charge;
                break;
            }
            case(Hybrid):
            {
                Allstation[Bike->Station].NetHybrid+=charge;
                break;
            }
            }
            cout<<"Rental charge for this bike is " <<  charge <<"."<< endl;
        }
        else if(cmd == "Inquire")
        {
            LicenseType License;
            cin >> License;
            Inquire(License);
        }
        else if(cmd == "StationReport")
        {
            string x;
            StationType Station;
            cin >> x;
            Station = stoST(x);
            StationReport(Station);
        }
        else if(cmd == "UbikeReport")
        {
            UbikeReport();
        }
        else if(cmd == "NetSearch")
        {
            string x;
            StationType Station;
            cin >> x;
            Station = stoST(x);
            NetSearch(Station);
        }
        else if(cmd == "HashReport")
        {
            HashReport();
        }
    }
    return 0;
}
StationType stoST(string s,int a)
{
    StationType tmp;
    if(s=="Danshui"||a==0) tmp=Danshui;
    else if(s=="Hongshulin"||a==1) tmp=Hongshulin;
    else if(s=="Beitou"||a==2) tmp=Beitou;
    else if(s=="Shilin"||a==3) tmp=Shilin;
    else if(s=="Zhongshan"||a==4) tmp=Zhongshan;
    else if(s=="Xinpu"||a==5) tmp=Xinpu;
    else if(s=="Ximen"||a==6) tmp=Ximen;
    else if(s=="Liuzhangli"||a==7) tmp=Liuzhangli;
    else if(s=="Muzha"||a==8) tmp=Muzha;
    else if(s=="Guting"||a==9) tmp=Guting;
    else if(s=="Gongguan"||a==10) tmp=Gongguan;
    else if(s=="Jingmei"||a==11) tmp=Jingmei;
    return tmp;
}
ClassType   stoClassType(string in)
{
    ClassType out;
    if(in == "Electric") out = Electric;
    else if(in == "Lady") out = Lady;
    else if(in == "Road") out = Road;
    else if(in == "Hybrid") out = Hybrid;
    return out;
}
int   inttoLicenseType(LicenseType in)
{
    int x[5]= {},s[5]= {},tmp=0;
    for(int i=0; i<5; i++)
    {
        if(in[i] >= 48 && in[i] <= 57)
            x[i] = in[i] - 48;
        else if(in[i] >= 65 && in[i] <= 90)
            x[i] = in[i] - 55;
    }
    s[0] = x[0];
    for(int i=1; i<5; i++)
        s[i] = s[i-1] * 31 +x[i];
    vector<int> b;
    while(s[4])
    {
        b.push_back(s[4]%2);
        s[4]=s[4]/2;
    }
    for(int i=10; i<18 && i<b.size(); i++)
        tmp+= b[i] * pow(2,(i-10));
    return tmp;
}
BikePtr NewBike(LicenseType L, int Mile, ClassType Class, StationType Station)
{
    switch(Class)
    {
    case Electric:
    {
        Allstation[Station].HElectric.Elem[Allstation[Station].HElectric.Number] = new BikeType;
        BikePtr ptr=Allstation[Station].HElectric.Elem[Allstation[Station].HElectric.Number];
        ptr->Class = Class;
        strcpy(ptr->License,L);
        ptr->Mileage = Mile;
        ptr->Station = Station;
        ptr->Status = Free;
        Allstation[Station].NumElectric++;
        ptr->Cursor = Allstation[Station].NumElectric-1;
        Sortheap(Allstation[Station].HElectric,ptr->Cursor, 0);
        hashnew(L,ptr,inttoLicenseType(L),0);
        return ptr;
    }
    case Lady:
    {
        Allstation[Station].HLady.Elem[Allstation[Station].HLady.Number] = new BikeType;
        BikePtr ptr=Allstation[Station].HLady.Elem[Allstation[Station].HLady.Number];
        ptr->Class = Class;
        strcpy(ptr->License,L);
        ptr->Mileage = Mile;
        ptr->Station = Station;
        ptr->Status = Free;
        Allstation[Station].NumLady++;
        ptr->Cursor = Allstation[Station].NumLady-1;
        Sortheap(Allstation[Station].HLady, ptr->Cursor, 0);
        hashnew(L,ptr,inttoLicenseType(L),0);
        return ptr;
    }
    case Road:
    {
        Allstation[Station].HRoad.Elem[Allstation[Station].HRoad.Number] = new BikeType;
        BikePtr ptr=Allstation[Station].HRoad.Elem[Allstation[Station].HRoad.Number];
        ptr->Class = Class;
        strcpy(ptr->License,L);
        ptr->Mileage = Mile;
        ptr->Station = Station;
        ptr->Status = Free;
        Allstation[Station].NumRoad++;
        ptr->Cursor = Allstation[Station].NumRoad-1;
        Sortheap(Allstation[Station].HRoad, ptr->Cursor, 0);
        hashnew(L,ptr,inttoLicenseType(L),0);
        return ptr;
    }
    case Hybrid:
    {
        Allstation[Station].HHybrid.Elem[Allstation[Station].HHybrid.Number] = new BikeType;
        BikePtr ptr=Allstation[Station].HHybrid.Elem[Allstation[Station].HHybrid.Number];
        ptr->Class = Class;
        strcpy(ptr->License,L);
        ptr->Mileage = Mile;
        ptr->Station = Station;
        ptr->Status = Free;
        Allstation[Station].NumHybrid++;
        ptr->Cursor = Allstation[Station].NumHybrid-1;
        Sortheap(Allstation[Station].HHybrid,ptr->Cursor, 0);
        hashnew(L,ptr,inttoLicenseType(L),0);
        return ptr;
    }
    }
}
BikePtr SearchBike(LicenseType L)
{
    int num = inttoLicenseType(L);
    HNodeType *ptr = &(Hash.table[num]);
    while(ptr&&(strcmp(ptr->License,L)!=0))
        ptr = ptr->next_ptr;
    if(ptr)
        return ptr->Ptr;
    else
        return 0;
}
int   JunkBikePtr(BikePtr Bike)
{
    if (!Bike)
        return 2;
    else if (Bike->Status==Rented)
        return 1;
    else
    {
        int &num = DealwithNum(Bike->Station,Bike->Class);
        num--;
        HeapType &h=Dealwithclass(Bike->Class,Bike->Station);
        h.Number--;
        hashnew(Bike->License,Bike,inttoLicenseType(Bike->License),1);
        if(Bike->Cursor==h.Number)
        {
            h.Elem[Bike->Cursor]=0;
        }
        else
        {
            h.Elem[Bike->Cursor] = h.Elem[h.Number];
            h.Elem[Bike->Cursor]->Cursor = Bike->Cursor;
            h.Elem[h.Number]=0;
            Sortheap(h,Bike->Cursor,1);
        }
        delete Bike;
        return 0;
    }
}
void  Sortheap(HeapType &Heap, int num, int mode)   //mode 0:bubble up, mode 1:bubble up+down
{
    switch (mode)
    {
    case 0:
    {
        if(num>0)
        {
            while((((num-1)/2)>=0)&&Heap.Elem[num]->Mileage>Heap.Elem[(num-1)/2]->Mileage)
            {
                swap(Heap.Elem[num]->Cursor,Heap.Elem[(num-1)/2]->Cursor);
                swap(Heap.Elem[num],Heap.Elem[(num-1)/2]);
                num=(num-1)/2;
            }
        }
        Heap.Number++;
        break;
    }
    case 1:
    {
        if(num>0)
        {
            while(Heap.Elem[num]->Mileage>Heap.Elem[(num-1)/2]->Mileage)
            {
                swap(Heap.Elem[num]->Cursor,Heap.Elem[(num-1)/2]->Cursor);
                swap(Heap.Elem[num],Heap.Elem[(num-1)/2]);
                num=(num-1)/2;
            }
        }
        BikePtr left,right;
        if((num*2+1)<Heap.Number)left=Heap.Elem[num*2+1];
        else
        {
            left = 0;
            right=0;
        }
        if((num*2+2)<Heap.Number)right=Heap.Elem[num*2+2];
        while (left||right)
        {
            if((num*2+1)<Heap.Number)left=Heap.Elem[num*2+1];
            else
            {
                left = 0;
                right=0;
            }
            if((num*2+2)<Heap.Number)right=Heap.Elem[num*2+2];
            else right = 0;
            if(left&&right)
            {
                if(left->Mileage>right->Mileage)
                {
                    if(left->Mileage>Heap.Elem[num]->Mileage)
                    {
                        swap(Heap.Elem[num]->Cursor,Heap.Elem[num*2+1]->Cursor);
                        swap(Heap.Elem[num],Heap.Elem[num*2+1]);
                        num=(num*2+1);
                    }
                    else break;
                }
                else if(left->Mileage==right->Mileage)
                {
                    if(left->Mileage>Heap.Elem[num]->Mileage)
                    {
                        swap(Heap.Elem[num]->Cursor,Heap.Elem[num*2+1]->Cursor);
                        swap(Heap.Elem[num],Heap.Elem[num*2+1]);
                        num=(num*2+1);
                    }
                    else break;
                }
                else
                {
                    if(right->Mileage>Heap.Elem[num]->Mileage)
                    {
                        swap(Heap.Elem[num]->Cursor,Heap.Elem[num*2+2]->Cursor);
                        swap(Heap.Elem[num],Heap.Elem[num*2+2]);
                        num=(2*num+2);
                    }
                    else break;
                }
            }
            else if(left)
            {
                if(left->Mileage>Heap.Elem[num]->Mileage)
                {
                    swap(Heap.Elem[num]->Cursor,Heap.Elem[num*2+1]->Cursor);
                    swap(Heap.Elem[num],Heap.Elem[num*2+1]);
                    num=num*2+1;
                }
                else break;
            }
            else break;
        }
    }
    }
}
void  hashnew(LicenseType L,BikePtr Bike, int num,int mode) //mode 0: insert bike into hash system, mode 1:delete bike from hash system
{
    switch(mode)
    {
    case 0:
    {
        HNodeType *ptr=&(Hash.table[num]);
        if(ptr->Ptr)
        {
            while(ptr->next_ptr)
                ptr = ptr->next_ptr;
            ptr->next_ptr = new HNodeType;
            ptr->next_ptr->Ptr =Bike;
            strcpy(ptr->next_ptr->License,L);
        }
        else
        {
            ptr->Ptr = Bike;
            strcpy(ptr->License,L);
        }
        Hash.BikeNum++;
        break;
    }
    case 1:
    {
        HNodeType *p = new HNodeType;
        p->next_ptr = &(Hash.table[num]);
        if(strcmp(Hash.table[num].License,L)==0)
        {
            if(Hash.table[num].next_ptr)Hash.table[num]=*(Hash.table[num].next_ptr);
            else
            {
                Hash.table[num].Ptr=0;
            }
        }
        else
        {
            while(strcmp(p->next_ptr->License,L)!=0)
            {
                p=p->next_ptr;
            }
            p->next_ptr=p->next_ptr->next_ptr;
        }
        Hash.BikeNum--;
        break;
    }
    }
}
void  TransBikePtr(StationType Station, BikePtr Bike)
{
    if(Bike->Status==Rented)
    {
        cout<<"Bike "<<Bike->License<<" is now being rented."<<endl;
    }
    else
    {
        HeapType &from = Dealwithclass(Bike->Class,Bike->Station);
        HeapType &to   = Dealwithclass(Bike->Class,Station);
        if(Bike->Cursor==(from.Number-1))
        {
            from.Elem[from.Number-1]=0;
            from.Number--;
        }
        else
        {
            from.Elem[Bike->Cursor]=from.Elem[from.Number-1];
            from.Elem[Bike->Cursor]->Cursor = Bike->Cursor;
            from.Elem[from.Number-1]=0;
            from.Number--;
            Sortheap(from,Bike->Cursor,1);
        }
        int &num=DealwithNum(Bike->Station,Bike->Class);
        num--;
        int &num2=DealwithNum(Station,Bike->Class);
        num2++;
        to.Elem[to.Number]=Bike;
        Bike->Station=Station;
        Bike->Cursor=to.Number;
        Sortheap(to,Bike->Cursor,0);
        cout<<"Bike "<<Bike->License<<" is transferred to ";
        string s=Dealwithstation(Bike->Station);
        cout<<s<<"."<<endl;
    }
}
HeapType& Dealwithclass(ClassType c,StationType s)
{
    switch (c)
    {
    case Electric:
        return Allstation[s].HElectric;
    case Lady:
        return Allstation[s].HLady;
    case Road:
        return Allstation[s].HRoad;
    case Hybrid:
        return Allstation[s].HHybrid;
    }
}
string Dealwithstation(StationType s)
{
    switch (s)
    {
    case Danshui:
        return"Danshui";
    case Hongshulin:
        return"Hongshulin";
    case Beitou:
        return"Beitou";
    case Shilin:
        return"Shilin";
    case Zhongshan:
        return"Zhongshan";
    case Xinpu:
        return"Xinpu";
    case Ximen:
        return"Ximen";
    case Liuzhangli:
        return"Liuzhangli";
    case Muzha:
        return"Muzha";
    case Guting:
        return"Guting";
    case Gongguan:
        return"Gongguan";
    case Jingmei:
        return"Jingmei";
    }
}
void  RentBikePtr(StationType s, BikePtr Bike)
{
    HeapType &rent = Allstation[s].HRent;
    HeapType &heap = Dealwithclass(Bike->Class, s);
    Bike->Status=Rented;
    rent.Elem[rent.Number]=Bike;
    Bike->Cursor=rent.Number;
    Sortheap(rent,Bike->Cursor,0);
    ///////////////////class heap ////////////////////////
    if(heap.Number>1)
    {
        heap.Elem[0] = heap.Elem[(heap.Number-1)];
        heap.Elem[(heap.Number-1)]= 0;
        heap.Number--;
        heap.Elem[0]->Cursor = 0;
        Sortheap(heap,0,1);
    }
    else
    {
        heap.Elem[(heap.Number-1)]= 0;
        heap.Number--;
    }
}
int   Returns(StationType s,BikePtr Bike, int ReturnMile)
{
    int run = (ReturnMile - Bike->Mileage);
    Bike->Mileage = ReturnMile;
    Bike->Status  = Free;
    int cur = Bike->Cursor;
    HeapType &rent = Allstation[Bike->Station].HRent;
    rent.Number--;
    if(rent.Number==cur)
    {
        rent.Elem[cur]=0;
    }
    else
    {
        rent.Elem[cur]=rent.Elem[rent.Number];
        rent.Elem[cur]->Cursor = cur;
        rent.Elem[rent.Number]=0;
        Sortheap(rent, cur, 1);
    }
    HeapType &heap = Dealwithclass(Bike->Class, Bike->Station);
    Bike->Cursor = heap.Number;
    heap.Elem[Bike->Cursor] = Bike;
    Sortheap(heap,Bike->Cursor,0);
    int charge = Dealwithcharge(Bike->Class,Bike->Station,s, run);
    return charge;
}
int   Dealwithcharge(ClassType c,StationType start,StationType en,int Mile)
{
    if (Mile>Map.a[start][en])
    {
        switch(c)
        {
        case Electric:
            return (Mile*40);
        case Lady:
            return (Mile*30);
        case Road:
            return (Mile*20);
        case Hybrid:
            return (Mile*25);
        }
    }
    else
    {
        switch(c)
        {
        case Electric:
            return (Mile*30);
        case Lady:
            return (Mile*25);
        case Road:
            return (Mile*15);
        case Hybrid:
            return (Mile*20);
        }
    }
}
void  Inquire(LicenseType License)
{
    BikePtr Bike = SearchBike(License);
    if (!Bike)
    {
        cout <<"Bike "<<License<<" does not belong to our company."<<endl;
    }
    else
    {
        cout << setw(15) << "License" << setw(15) << "Mileage"<< setw(15) << "Class" << setw(15) <<"Station"<<endl;
        for (int i=0; i<60; i++)cout<<"=";
        cout <<endl;
        cout << setw(15) << License
             << setw(15) << Bike->Mileage;
        string clazz;
        switch(Bike->Class)
        {
        case(Electric):
        {
            clazz="Electric";
            break;
        }
        case(Lady):
        {
            clazz="Lady";
            break;
        }
        case(Road):
        {
            clazz="Road";
            break;
        }
        case(Hybrid):
        {
            clazz="Hybrid";
            break;
        }
        }
        cout<< setw(15) << clazz;
        cout<< setw(15) << Dealwithstation(Bike->Station) <<endl<<endl;
    }
}
void  HashReport()
{

    cout <<"Hash Table"<<endl;
    HNodeType *p;
    for (int i=0; i<256; i++)
    {
        p=&(Hash.table[i]);
        if(p->Ptr)
        {
            cout <<i<<" "<<p->License;
            p = p->next_ptr;
            while(p)
            {
                cout <<"->"<<p->License;
                p = p->next_ptr;
            }
            cout<<endl;
        }
    }
}
void  StationReport (StationType s)
{
    string x = Dealwithstation(s);
    cout << setw(30) <<x<<endl;
    cout << setw(30) <<"Free Bikes"<<endl;
    cout << setw(15) << "License" << setw(15) << "Mileage"<< setw(15) << "Class" << setw(15) <<"SubTotal"<<endl;
    for (int i=0; i<60; i++)cout<<"=";
    int c = Printfree(s,15,0);
    cout<<endl;
    for (int i=0; i<60; i++)cout<<"=";
    cout << endl;
    cout <<setw(60) <<c<<endl<<endl;
    cout << setw(30) <<"Rented Bikes"<<endl;
    cout << setw(15) << "License" << setw(15) << "Mileage"<< setw(15) << "Class" << setw(15) <<"SubTotal"<<endl;
    for (int i=0; i<60; i++)cout<<"=";
    c = Printrented(s,15,0);
    cout << endl;
    for (int i=0; i<60; i++)cout<<"=";
    cout << endl;
    cout <<setw(60) <<c<<endl<<endl;
    cout<<setw(12)<<"Net"<<setw(12)<<"Electric"<<setw(12)<<"Lady"<<setw(12)<<"Road"<<setw(12)<<"Hybrid"<<endl;
    for (int i=0; i<60; i++)cout<<"=";
    cout << endl;
    cout<<setw(12)<<Allstation[s].Net
        <<setw(12)<<Allstation[s].NumElectric
        <<setw(12)<<Allstation[s].NumLady
        <<setw(12)<<Allstation[s].NumRoad
        <<setw(12)<<Allstation[s].NumHybrid<<endl;
    for (int i=0; i<60; i++)cout<<"=";
    cout<<endl<<endl;
}
void  UbikeReport()
{
    cout << setw(30) <<"Taipei U-bike"<<endl;
    cout << setw(30) <<"Free Bikes"<<endl;
    cout << setw(12) << "License" << setw(12) << "Mileage"<< setw(12) << "Class" << setw(12) <<"Station"<<setw(12)<<"Total"<<endl;
    for (int i=0; i<60; i++)cout<<"=";
    int c=0;
    for (int i=0; i<12; i++)
        c += Printfree(stoST("",i),12,1);
    cout<<endl;
    for (int i=0; i<60; i++)cout<<"=";
    cout<<endl;
    cout <<setw(60)<<c<<endl;
    cout<<endl;
    cout << setw(30) <<"Rented Bikes"<<endl;
    cout << setw(12) << "License" << setw(12) << "Mileage"<< setw(12) << "Class" << setw(12) <<"Station"<<setw(12)<<"Total"<<endl;
    for (int i=0; i<60; i++)cout<<"=";
    c=0;
    for (int i=0; i<12; i++)
        c+= Printrented(stoST("",i), 12, 1);
    cout<<endl;
    for (int i=0; i<60; i++)cout<<"=";
    cout<<endl;
    cout <<setw(60)<<c<<endl;
    cout<<endl;
    cout<<setw(12)<<"Net"<<setw(12)<<"Electric"<<setw(12)<<"Lady"<<setw(12)<<"Road"<<setw(12)<<"Hybrid"<<endl;
    for (int i=0; i<60; i++)cout<<"=";
    cout<<endl;
    int net=0,ele=0,lad=0,roa=0,hyb=0;
    for (int i=0; i<12; i++)
    {
        net+=Allstation[stoST("",i)].Net;
        ele+=Allstation[stoST("",i)].NumElectric;
        lad+=Allstation[stoST("",i)].NumLady;
        roa+=Allstation[stoST("",i)].NumRoad;
        hyb+=Allstation[stoST("",i)].NumHybrid;
    }
    cout<<setw(12)<<net<<setw(12)<<ele<<setw(12)<<lad<<setw(12)<<roa<<setw(12)<<hyb<<endl;
    for (int i=0; i<60; i++)cout<<"=";
    cout<<endl<<endl;
}
int   Printfree (StationType s, int a,bool st)
{
    int c=0;
    string clazz;
    for (int i=0; i<Allstation[s].HElectric.Number; i++)
    {
        cout << endl;
        cout << setw(a) << Allstation[s].HElectric.Elem[i]->License
             << setw(a) << Allstation[s].HElectric.Elem[i]->Mileage;
        switch(Allstation[s].HElectric.Elem[i]->Class)
        {
        case(Electric):
        {
            clazz="Electric";
            break;
        }
        case(Lady):
        {
            clazz="Lady";
            break;
        }
        case(Road):
        {
            clazz="Road";
            break;
        }
        case(Hybrid):
        {
            clazz="Hybrid";
            break;
        }
        }
        cout<< setw(a) << clazz;
        if(st)cout<<setw(a)<<Dealwithstation(Allstation[s].HElectric.Elem[i]->Station);
        c++;
    }
    for (int i=0; i<Allstation[s].HLady.Number; i++)
    {
        cout<<endl;
        cout << setw(a) << Allstation[s].HLady.Elem[i]->License;
        cout << setw(a) << Allstation[s].HLady.Elem[i]->Mileage;
        switch(Allstation[s].HLady.Elem[i]->Class)
        {
        case(Electric):
        {
            clazz="Electric";
            break;
        }
        case(Lady):
        {
            clazz="Lady";
            break;
        }
        case(Road):
        {
            clazz="Road";
            break;
        }
        case(Hybrid):
        {
            clazz="Hybrid";
            break;
        }
        }
        cout<< setw(a) << clazz;
        if(st)cout<<setw(a)<< Dealwithstation(Allstation[s].HLady.Elem[i]->Station);
        c++;
    }
    for (int i=0; i<Allstation[s].HRoad.Number; i++)
    {
        cout<<endl;
        cout << setw(a) << Allstation[s].HRoad.Elem[i]->License;
        cout << setw(a) << Allstation[s].HRoad.Elem[i]->Mileage;
        switch(Allstation[s].HRoad.Elem[i]->Class)
        {
        case(Electric):
        {
            clazz="Electric";
            break;
        }
        case(Lady):
        {
            clazz="Lady";
            break;
        }
        case(Road):
        {
            clazz="Road";
            break;
        }
        case(Hybrid):
        {
            clazz="Hybrid";
            break;
        }
        }
        cout<< setw(a) << clazz;
        if(st)cout<<setw(a)<< Dealwithstation(Allstation[s].HRoad.Elem[i]->Station);
        c++;
    }
    for (int i=0; i<Allstation[s].HHybrid.Number; i++)
    {
        cout<<endl;
        cout << setw(a) << Allstation[s].HHybrid.Elem[i]->License;
        cout << setw(a) << Allstation[s].HHybrid.Elem[i]->Mileage;
        switch(Allstation[s].HHybrid.Elem[i]->Class)
        {
        case(Electric):
        {
            clazz="Electric";
            break;
        }
        case(Lady):
        {
            clazz="Lady";
            break;
        }
        case(Road):
        {
            clazz="Road";
            break;
        }
        case(Hybrid):
        {
            clazz="Hybrid";
            break;
        }
        }
        cout<< setw(a) << clazz;
        if(st)cout<<setw(a)<<Dealwithstation(Allstation[s].HHybrid.Elem[i]->Station);
        c++;
    }
    return c;
}
int   Printrented (StationType s,int a,bool st)
{
    int c=0;
    string clazz,staz;
    for (int i=0; i<Allstation[s].HRent.Number; i++)
    {
        cout<<endl;
        cout << setw(a) <<Allstation[s].HRent.Elem[i]->License;
        cout << setw(a) <<Allstation[s].HRent.Elem[i]->Mileage;
        switch(Allstation[s].HRent.Elem[i]->Class)
        {
        case(Electric):
        {
            clazz="Electric";
            break;
        }
        case(Lady):
        {
            clazz="Lady";
            break;
        }
        case(Road):
        {
            clazz="Road";
            break;
        }
        case(Hybrid):
        {
            clazz="Hybrid";
            break;
        }
        }
        cout<< setw(a) << clazz;
        if(st)
        {
            staz= Dealwithstation(Allstation[s].HRent.Elem[i]->Station);
            cout<<setw(a)<<staz;
        }
        c++;
    }
    return c;
}
void  NetSearch(StationType s)
{
    cout<<Dealwithstation(s)<<endl;
    for (int i=0; i<15; i++)cout<<"=";
    cout<<endl;
    cout << "Electric "<< Allstation[s].NetElectric<<endl;
    cout << "Lady "    << Allstation[s].NetLady<<endl;
    cout << "Road "    << Allstation[s].NetRoad<<endl;
    cout << "Hybrid "  << Allstation[s].NetHybrid<<endl;
    for (int i=0; i<15; i++)cout<<"=";
    cout <<endl;
    cout << "Total "   << Allstation[s].Net<<endl<<endl;
}
int&  DealwithNum (StationType s,ClassType c)
{
    switch(c)
    {
    case Electric:
        return Allstation[s].NumElectric;
    case Lady:
        return Allstation[s].NumLady;
    case Road:
        return Allstation[s].NumRoad;
    case Hybrid:
        return Allstation[s].NumHybrid;
    }
}
int   Graph::choose(const int n)
{
    int prevmax = -1;
    int index = -1;
    for (int i = 0; i < n; i++)
        if ( (!s[i]) && ((prevmax == -1) || (dist[i] < prevmax)) )
        {
            prevmax = dist[i];
            index = i;
        }
    return index;
}
void  Graph::ShortestPath(const int n, const int v)
{
    for (int i = 0; i < n; i++)
    {
        s[i] = false;    // initialize
        dist[i] = length[v][i];
    }
    s[v] = true;
    dist[v] = 0;

    for (int i = 0; i < n-2; i++)   // determine @n-1@ paths from vertex @v@
    {
        int u = choose(n);  // @choose@ returns a value @u@:
        // @dist[u]@ = minimum @dist[w]@, where @s[w]@ = FALSE
        s[u] = true;
        for (int w = 0; w < n; w++)
            if (! s[w])
                if (dist[u] + length[u][w] < dist[w])
                    dist[w] = dist[u] + length[u][w];
    }
}
