# 
[YouBike-Rental-System] (https://github.com/gary1346aa/YouBike-Rental-System/blob/master/Slides_2015.pdf)

## Available Functions

### 1. NewBike Class License Mile StationName
Example:
```
Input : NewBike Lady 00000 0 Hongshulin
Output : New bike is received by Station Hongshulin.
```
### 2. JunkIt License
Example:
1. General case.
```
Input : JunkIt TPI
Output : Bike TPI01 is deleted from Danshui.
```
2. If the bike is now being rented.
```
Input : JunkIt TPI
Output : Bike TPI03 is now being rented.
```
3. If the bike doesn't exist.
```
Input : JunkIt KKK
Output : Bike KKK23 does not belong to our company.
```
### 3. Rent StationName Class
Example:
1. If there is no bike of specific class in the station.
```
Input : Rent Danshui Lady
Output : No free bike is available.
```
2. If there exists a bike of specific class in the station.
```
Input : Rent Danshui Lady
Output : A bike is rented from Danshui.
```
### 4. Returns StationName License Mile
Example:
```
Input : Returns Hongshulin 00A03 190
Output : Rental charge for this bike is 1245.
```
### 5. Trans StationName License
Example:
1. General case.
```
Input : Trans Jingmei 00A
Output : Bike 00A09 is transferred to Jingmei.
```
2. If the bike doesn't exist.
```
Input : Trans Jingmei 0FA
Output : Bike 0FA09 does not belong to our company.
```

3. If bike is now being rented.
```
Input : Trans Jingmei 0AA
Output : Bike 0AA09 is now being rented.
```
### 6. Inquire License
Example:
1. If the bike does not exist.
```
Input : Inquire 00BC
Output : Bike 00BC4 does not belong to our company.
```
2. General case.
```
Input : Inquire 00BC
```

### 7. Station Report StationName
Print StationName and the following kinds of heaps in it. Each heap is printed from its
first node to the last one.
(1)Free Bike Heaps:
Print “Free Bikes” and the heaps from Electric=0 to Hybrid=3.
(2)Rented Bike Heap:
Print “Rented Bikes” and then HRent.

Example:
```
Input : StationReport Danshui
```

### 8. UbikeReport
Example:
```
Input : UbikeReport
```

### 9. NetSearch StationName
Example:
```
Input : NetSearch Danshui
```

### 10. HashReport
Example:
```
Input : HashReport
```
