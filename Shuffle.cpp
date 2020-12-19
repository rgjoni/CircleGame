#include <iostream>
#include <vector>
#include <array>
using namespace std;
void initarr(int arr[],int size)
{
    for(int i =0;i<size;i++)
    {
        arr[i] = 0;
    }
}
void printarr(int arr[], int size)
{
    for(int i =0;i<size;i++)
    {
        cout<< arr[i];
    }
    cout<< "\n";
}
void shuffle(int arr[],int size)
{
    vector<int> shuffle;
    for(int i=0;i<size;i++)
    {
       shuffle.push_back(i);
    }
    srand(time(NULL));
    int count =0;
    while(!shuffle.empty())
    {
    int pos = rand() % shuffle.size();
    arr[count] = shuffle[pos];
    shuffle.erase(shuffle.begin() + pos);
    count++;
    //cout<<pos;
    } 
}
int main()
{   
    cout<< "Enter your size of the array to be shuffled";
    int size;
    cin>>size;
    while(size<0)
    {
        cout<<"Size cannot be zero, renter";
        cin>>size;
    }
    int arr[size];
    initarr(arr,size);
    printarr(arr,size);
    shuffle(arr,size);
    printarr(arr,size);
    return 0;
}
