# YouBike-Rental-System

# Yo u h a v e t o p r i n t a l l o u t p u t i n t h e o u t p u t

# file specified by main arguments, or you

# won’t get any score.

Output format for Data Structure Final Project

1. NewBike Class License Mile StationName
Example:
Input : NewBike Lady 00000 0 Hongshulin
Output : New bike is received by Station Hongshulin.
2. JunkIt License
Example:
1. General case.
Input : JunkIt TPI
Output : Bike TPI01 is deleted from Danshui.
2. If the bike is now being rented.
Input : JunkIt TPI
Output : Bike TPI03 is now being rented.
3. If the bike doesn't exist.
Input : JunkIt KKK
Output : Bike KKK23 does not belong to our company.
3. Rent StationName Class
Example:
1. If there is no bike of specific class in the station.
Input : Rent Danshui Lady
Output : No free bike is available.
2. If there exists a bike of specific class in the station.
Input : Rent Danshui Lady
Output : A bike is rented from Danshui.
4. Returns StationName License Mile
Example:
Input : Returns Hongshulin 00A03 190
Output : Rental charge for this bike is 1245.
5. Trans StationName License
Example:
1. General case.
Input : Trans Jingmei 00A
Output : Bike 00A09 is transferred to Jingmei.
2. If the bike doesn't exist.
Input : Trans Jingmei 0FA
Output : Bike 0FA09 does not belong to our company.


3. If bike is now being rented.
Input : Trans Jingmei 0AA
Output : Bike 0AA09 is now being rented.
6. Inquire License
output format :

Hint: (c++)
#include<iomanip> //using setw()
std::cout << setw(15) << "License" << setw(15) << "Mileage"<< .......

Example:

1. If the bike does not exist.
Input : Inquire 00BC
Output : Bike 00BC4 does not belong to our company.
2. General case.
Input : Inquire 00BC


7. StationReport StationName
Print StationName and the following kinds of heaps in it. Each heap is printed from its
first node to the last one.
(1)Free Bike Heaps:
Print “Free Bikes” and the heaps from Electric=0 to Hybrid=3.
(2)Rented Bike Heap:
Print “Rented Bikes” and then HRent.

Example:
Input : StationReport Danshui


8. UbikeReport
Yo u n e e d t o p r i n t t h e d a t a i n t h e o r d e r.
(1)Free Bikes:
Print the 4 heaps (from Electric=0 to Hybrid=3) of free bikes in each station (from
Danshui=0 to Jingmei=11). Each heap is printed from its first node to the last one.
(2)Rented Bikes:
    Print the heaps (from Electric=0 to Hybrid=3) of rented bikes in each station (from
    Danshui=0 to Jingmei=11). The heap is printed from its first node to the last one.
Example:
Input : UbikeReport


9. NetSearch StationName

(1) Print StationName
(2) 15’=’
(3) Print 4 type class net from Electric=0 to Hybrid=3.
(4) 15’=’
(5) Print total net
(6) newline

Example
Input : NetSearch Danshui
Output :

10. HashReport
Print the hash table using linked list.

Example:
Input : HashReport


