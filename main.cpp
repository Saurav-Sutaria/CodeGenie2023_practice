#include<bits/stdc++.h>
using namespace std;

class Car{
    public:
    string number;
    string checkinTime;
    string checkoutTime;
    string category;
    string parkedLocation;
    int parkingFloor;
    int parkingSlot;
    int charges;

    Car(string number,string checkinTime,string category){
        this -> number = number;
        this -> checkinTime = checkinTime;
        this -> category = category;
    }

};

vector<string> split(string s){
    vector<string> ans;
    int start = 0;
    for(int i=0;i<s.size();i++){
        if(s[i] == ' '){
            ans.push_back(s.substr(start,i-start));
            start = i+1;
        }
    }
    ans.push_back(s.substr(start));
    return ans;
}
pair<int,int> getSlot(vector<vector<int>>& parking,string category){
    if(category == "R"){
        for(int i=0;i<parking.size();i++){
            for(int j=1;j<parking[i].size();j++){
                if(parking[i][j] == 2){
                    return make_pair(i,j);
                }
            }
        }
    }
    else if(category == "NR"){
        for(int i=0;i<parking.size();i++){
            for(int j=1;j<parking[i].size();j++){
                if(parking[i][j] == 0){
                    return make_pair(i,j);
                }
            }
        }
    }
    return make_pair(-1,-1);
}
void bookSlot(vector<vector<int>>& parking,pair<int,int> slot, string category){
    if(category == "R"){
        parking[slot.first][slot.second] = 3;
    }else parking[slot.first][slot.second] = 1;
    return;
}
void getCarDetails(Car* c){
    cout<<c->parkedLocation<<","<<c->number<<","<<c->checkinTime<<","<<c->checkoutTime<<","<<c->charges<<","<<c->category<<endl;
}
void freeTheSlot(vector<vector<int>>& parking, Car* c){
    if(c->category == "R"){
        parking[c->parkingFloor][c->parkingSlot] = 2;
    }else parking[c->parkingFloor][c->parkingSlot] = 0;
    return;
}
void printParking(vector<vector<int>>& parking){
    for(auto i:parking){
        for(auto j:i){
            cout<<j<<" ";
        }cout<<endl;
    }
}
int generateBill(Car* c){
    int h1 = stoi(c->checkinTime.substr(0,2));
    int m1 = stoi(c->checkinTime.substr(3,2));
    if(c->checkinTime.substr(5,2) == "pm") h1 += 12;
    int h2 = stoi(c->checkoutTime.substr(0,2));
    int m2 = stoi(c->checkoutTime.substr(3,2));
    if(c->checkoutTime.substr(5,2) == "pm") h2 += 12;

    int totalTime = (h2-h1) + ((m2-m1)/60);
    cout<<"totaltime"<<totalTime<<endl;
    if(totalTime <= 2) return 50;
    else if(totalTime >= 2 && totalTime <= 4) return 80;
    else return 100;
}

int main(){
    int floors; 
    cin>>floors;

    int slots;
    cin>>slots;
    
    cin.ignore(); //for blank line
    
    string reserved;
    getline(cin,reserved);
    vector<string> reservedSlots = split(reserved);

    //database
    /*
    0 : non-reserved empty
    1 : non-reserved booked
    2 : reserved empty
    3 : reserved booked
    */
    vector<vector<int>> parking(floors,vector<int>(slots+1));

    //marking the reserved slots as 2
    for(auto i:reservedSlots){
        int tempFloor = (i[0] - 'A');
        int tempSlot = stoi(i.substr(2));
        parking[tempFloor][tempSlot] = 2;
    }

    for(auto i:parking){
        for(auto j:i){
            cout<<j<<" ";
        }
        cout<<endl;
    }
    //list of all cars;
    vector<Car*> carInfo;

    //maping each car with number plate and index
    unordered_map<string,int> mp;
    int idx = 0;

    //car inputs start
    while(true){
        string details; getline(cin,details);
        vector<string> currDetails = split(details);

        //checkin procedure
        if(currDetails.size() == 4){
            Car* c = new Car(currDetails[1],currDetails[2],currDetails[3]);
            carInfo.push_back(c);
            mp[c->number] = idx++;
            //finding free slot
            pair<int,int> freeSlot = getSlot(parking,currDetails[3]);
            if(freeSlot.first == -1){
                cout<<"No slot is empty\n";
                continue;
            }
            c -> parkingFloor = freeSlot.first;
            c-> parkingSlot = freeSlot.second;
            char ch = 'A' + freeSlot.first;
            string parked;
            parked.push_back(ch);
            parked += "-" + to_string(freeSlot.second);
            c-> parkedLocation = parked;
            bookSlot(parking,freeSlot ,currDetails[3]);
            printParking(parking);
        }
        //checkout procedure
        else if(currDetails.size() == 3){
            Car* currCar = carInfo[mp[currDetails[1]]];
            currCar->checkoutTime = currDetails[2];
            currCar->charges = generateBill(currCar);
            getCarDetails(currCar);
            freeTheSlot(parking,currCar);
        }
        
        //report generation
        else if(currDetails.size() == 2){
            cout<<"PARKING SLOT, CAR NO, CHECK IN TIME, CHECK OUT TIME, CHARGES, CATEGORY"<<endl;
            for(auto i:carInfo){
                getCarDetails(i);
            }
            break;
        }
        else{
            cout<<"INVALID INPUT"<<endl;
        } 
    }
    return 0;
}