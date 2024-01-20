#ifndef HASH_TABLE_HPP
#define HASH_TABLE_HPP

//=======================//
// Implemented Functions //
//=======================//
template<int MAX_SIZE>
int HashTable<MAX_SIZE>::PRIMES[3] = {102523, 100907, 104659};

template<int MAX_SIZE>
void HashTable<MAX_SIZE>::PrintLine(int tableIndex) const
{
    const HashData& data = table[tableIndex];

    // Using printf here it is easier to format
    if(data.sentinel == SENTINEL_MARK)
    {
        printf("[%03d]         : SENTINEL\n", tableIndex);
    }
    else if(data.sentinel == EMPTY_MARK)
    {
        printf("[%03d]         : EMPTY\n", tableIndex);
    }
    else
    {
        printf("[%03d] - [%03d] : ", tableIndex, data.lruCounter);
        printf("(%-5s) ", data.isCostWeighted ? "True" : "False");
        size_t sz = data.intArray.size();
        for(size_t i = 0; i < sz; i++)
        {
            if(i % 2 == 0)
                printf("[%03d]", data.intArray[i]);
            else
                printf("/%03d/", data.intArray[i]);

            if(i != sz - 1)
                printf("-->");
        }
        printf("\n");
    }
}

template<int MAX_SIZE>
void HashTable<MAX_SIZE>::PrintTable() const
{
    printf("____________________\n");
    printf("Elements %d\n", elementCount);
    printf("[IDX] - [LRU] | DATA\n");
    printf("____________________\n");
    for(int i = 0; i < MAX_SIZE; i++)
    {
        PrintLine(i);
    }
}

//=======================//
//          TODO         //
//=======================//
template<int MAX_SIZE>
int HashTable<MAX_SIZE>::Hash(int startInt, int endInt, bool isCostWeighted)
{
    int hash = 0;
    int arr[] = {startInt, endInt, isCostWeighted};
    for(int i = 0; i < 3; i++){
        hash += PRIMES[i] * arr[i];
    }
    /* TODO */
    return hash % MAX_SIZE;
}

template<int MAX_SIZE>
HashTable<MAX_SIZE>::HashTable()
{
    /* TODO */
    elementCount = 0;
    for(int i = 0; i < MAX_SIZE; i++){
        table[i].sentinel = EMPTY_MARK;
        table[i].lruCounter = 0;
    }
}

template<int MAX_SIZE>
int HashTable<MAX_SIZE>::Insert(const std::vector<int>& intArray, bool isCostWeighted)
{
    /* TODO */
    if(intArray.size() <= 0){
        throw InvalidTableArgException();
    }
    if(elementCount >= MAX_SIZE - 1 || elementCount * CAPACITY_THRESHOLD >= MAX_SIZE){
        throw TableCapFullException(elementCount);
    }
    int hash = Hash(intArray[0], intArray[intArray.size() - 1], isCostWeighted);
    int index = 0,temp = hash;
    while(table[temp].sentinel != SENTINEL_MARK && table[temp].sentinel != EMPTY_MARK){
        if(table[temp].startInt == intArray[0] && table[temp].endInt == intArray[intArray.size() - 1] && table[temp].isCostWeighted == isCostWeighted ){
            table[temp].lruCounter++;
            return table[temp].lruCounter -1 ;
        }
        temp = (hash + (index * index)) % MAX_SIZE;
        index++;
    }
    // değerleri gir
    table[temp].sentinel = OCCUPIED_MARK;
    table[temp].intArray = intArray;
    table[temp].startInt = intArray[0];
    table[temp].endInt = intArray[intArray.size() - 1];
    table[temp].isCostWeighted = isCostWeighted;
    table[temp].lruCounter++;
    elementCount++;
    return 0;
}

template<int MAX_SIZE>
bool HashTable<MAX_SIZE>::Find(std::vector<int>& intArray,
                               int startInt, int endInt, bool isCostWeighted,
                               bool incLRU)
{
     /* TODO*/ 
    int hash = Hash(startInt, endInt, isCostWeighted);
    int index = 0,temp = hash;
    while(table[temp].sentinel != EMPTY_MARK){
        if(table[temp].startInt == startInt && table[temp].endInt == endInt && table[temp].isCostWeighted == isCostWeighted){
            intArray = table[temp].intArray;
            if(incLRU)table[temp].lruCounter++;
            return true; 
        }
        temp = (hash + (index * index)) % MAX_SIZE;
        index++;
    }
    return false;
}

template<int MAX_SIZE>
void HashTable<MAX_SIZE>::InvalidateTable()
{
    /* TODO*/ 
    for(int i = 0; i < MAX_SIZE; i++){
            table[i].intArray.clear();
            table[i].sentinel = EMPTY_MARK;
            table[i].lruCounter = 0;
        
    }
    elementCount = 0;
}

template<int MAX_SIZE>
void HashTable<MAX_SIZE>::GetMostInserted(std::vector<int>& intArray) const
{
     /* TODO*/ 
    int max_ndex =0;
    for(int i = 1; i< MAX_SIZE; i++){
        if(table[max_ndex].lruCounter < table[i].lruCounter){
            max_ndex = i;
        }
    }
    intArray = table[max_ndex].intArray;
}

template<int MAX_SIZE>
void HashTable<MAX_SIZE>::Remove(std::vector<int>& intArray,
                                 int startInt, int endInt, bool isCostWeighted)
{
     /*TODO*/
    int hash = Hash(startInt, endInt, isCostWeighted);
    int index = 0,temp = hash;
    while(table[temp].sentinel != EMPTY_MARK){
        if(table[temp].startInt == startInt && table[temp].endInt == endInt && table[temp].isCostWeighted == isCostWeighted ){
            intArray = table[temp].intArray;
            table[temp].intArray.clear();
            table[temp].sentinel = SENTINEL_MARK;
            elementCount--;
            return;
        }
        temp = (hash + (index * index)) % MAX_SIZE;
        index++;
    }
}

template<int MAX_SIZE>
void HashTable<MAX_SIZE>::RemoveLRU(int lruElementCount)
{
     /*TODO*/ 
    if(lruElementCount >= MAX_SIZE){
        return;
    }
    for(int i = 0; i < lruElementCount; i++){
        int min_ndex = 0;
        for(int i = 1; i< MAX_SIZE; i++){
            
            if((table[min_ndex].lruCounter == 0 && table[i].lruCounter != 0) || (table[min_ndex].lruCounter > table[i].lruCounter && table[i].lruCounter != 0)){
                min_ndex = i;
            }
        }
        table[min_ndex].lruCounter = 0;
        table[min_ndex].intArray.clear();
        table[min_ndex].sentinel = SENTINEL_MARK;
        elementCount--;
    }
    
}

template<int MAX_SIZE>
void HashTable<MAX_SIZE>::PrintSortedLRUEntries() const
{
    /* TODO */
    /*std::priority_queue<Pair<int,int>> heap;
    std::vector<Pair<int,int>> pair;
    for(int i = 1; i< MAX_SIZE; i++){
        if(table[i].lruCounter != 0){
            Pair<int,int> pa;
            pa.key = table[i].lruCounter;
            pa.value = i;
            pair.push_back(pa);
        }
    }
    for(int i = 1; i < pair.size(); i++){
        heap.push(pair[i].key, pair[i].value);
    }
    while(!heap.empty()){
        PrintLine((heap.peek()).value);
        heap.pop();
    }*/

    MaxPairHeap<int, int> maxHeap;

    for (int i = 0; i < MAX_SIZE; ++i)
    {
        if(table[i].lruCounter != 0){
            maxHeap.push({table[i].lruCounter, i});
        }
    }

    while(!maxHeap.empty())
    {
        int maxIndex = maxHeap.top().value;
        maxHeap.pop();
        PrintLine(maxIndex);
    }
}


#endif // HASH_TABLE_HPP