#include <iostream>
#include <stack>
#include <assert.h>
#include <string>

using namespace std;

class Disk{
    public:
        Disk(size_t newSize) : size(newSize) {};
        bool operator<(const Disk& rhs) const{
            return this->size < rhs.size;
        }
    private:
        size_t size;
};

class Tower{
    public:
        Tower(string newName) : name(newName) {};
        size_t size(){
            return peg.size();
        }
        void push(const Disk& d){
            if(peg.size() > 0){
                assert(d < peg.top());
            }
            peg.push(d);
        }
        bool isLegal(Tower& to){
            if(this->peg.empty()){
                assert(!to.peg.empty());
                return false;
            }
            if(to.peg.empty()){
                return true;
            }
            return this->peg.top() < to.peg.top();
        }
        void move(Tower& to){
            to.peg.push(this->peg.top());
            this->peg.pop();
            cout << "Move a disk from: " << this->name << " to: " << to.name << endl;
        }
        void moveBidirectional(Tower& other){
            if(this->peg.empty() && other.peg.empty()){
                return;
            }
            (this->isLegal(other))? this->move(other) : other.move(*this);
        }
    private:
        string name;
        stack<Disk> peg;
};

void iterative(Tower& source, Tower& target, Tower& tmp){
    size_t height = source.size();

     while(!(target.size() == height || tmp.size() == height)){
        source.moveBidirectional(tmp);
        source.moveBidirectional(target);
        tmp.moveBidirectional(target);
    }
    if(tmp.size() == height){
        Tower copy("copy");
        copy = target;
        target = tmp;
        tmp = copy;
    }
}

void recursive(Tower& source, Tower& target, Tower& tmp, size_t height){
    if(height == 0){
        return;
    }
    if(height == 1){
        source.move(target);
        return;
    }
    recursive(source, tmp, target, height -1);    
    source.move(target);
    recursive(tmp, target, source, height -1);
}

int main(){
    cout << "Enter the tower height:";
    size_t height;
    cin >> height;
    Tower A("left tower"), B("middle tower"), C("right tower");
    int count = 0;

    for(size_t i = height; i > 0; i--){
        Disk d(count);
        A.push(Disk(i));
        count++;
    }
    assert(A.size() == height);
    recursive(A, B, C, height);
    assert(B.size() == height);
    assert(A.size() == 0);
    assert(C.size() == 0);
    return 0;
}
