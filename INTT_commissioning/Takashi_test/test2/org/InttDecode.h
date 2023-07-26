#ifndef __INTTDECODE_H__
#define __INTTDECODE_H__
#include <Event/fileEventiterator.h>
#include <Event/Event.h>
#include <Event/oncsSubevent.h>
#include <iomanip>
#include <vector>
#include <deque>

struct BcoHitBuf {
  BcoHitBuf():done(false){};

  void print(){
    cout<<"  BCO: 0x"<<hex<<bco<<dec<<endl;
    for(unsigned int i=0; i<vHit.size(); i++){
      cout<<"    hit: 0x"<<hex<<vHit[i]<<dec<<endl;
    }
    cout<<"    status: "<<(done?"done":"continue")<<endl;

  }

  Long64_t    bco;
  vector<int> vHit;
  bool        done;
};

struct EventBcoHitBuf{
    EventBcoHitBuf(Long64_t BCO): bco(BCO){
        for(int i=0; i<14; i++){
            bcoHitBuf[i] = NULL;
            status[i]    = false;
        }
    };

    void addBcoHitBuf(int ladder, BcoHitBuf* hitBuf){
      bcoHitBuf[ladder] = hitBuf;
      status[ladder]    = true;
    };

    bool checkStatus(){
        bool allStatus=true;
        for(int i=0; i<14; i++){
            allStatus &= status[i];
        }
        return allStatus;
    };

    void showStatus(){
        cout<<"   ";
        for(int i=0; i<14; i++){
            cout<<setw(3)<<status[i];
        }
        cout<<endl;
        cout<<"   ";
        for(int i=0; i<14; i++){
            int nhits=0;
            if((bcoHitBuf[i])!=NULL) { nhits = bcoHitBuf[i]->vHit.size(); }
            cout<<setw(3)<<nhits;
        }
        cout<<endl;
    };

    Long64_t   bco;
    BcoHitBuf* bcoHitBuf[14];
    bool       status[14];
};

/////////////
class InttPacket{
  public:
    InttPacket(EventBcoHitBuf* evtBcoHitBuf){
        for(int i=0; i<14; i++){ eventNum[i]=0;}
    
        decode(evtBcoHitBuf);
    };

    virtual ~InttPacket();

    void dump(ostream& os=cout);


    int      iValue(const int hit,const char * what);
    Long64_t lValue(const int hit,const char * what);

    int      iValue(const int hit, const int field);
    Long64_t lValue(const int hit, const int field);

  protected:
    int decode(EventBcoHitBuf* evtBcoHitBuf);

    struct intt_hit
    {
        uint64_t bco;
        uint16_t fee;
        uint16_t channel_id;
        uint16_t chip_id;
        uint16_t adc;
        uint16_t FPHX_BCO;
        uint16_t full_FPHX;
        uint16_t full_ROC;
        uint16_t amplitude;
        //uint16_t full_fphx;
        uint32_t word;
    };


    int eventNum[16];
    std::vector<intt_hit *> intt_hits;
};


/////////////
class InttDecode {
public:
    InttDecode(fileEventiterator* eventItr, int pID=3001) :
        packetID(pID), evtItr(eventItr)
      { };
    virtual ~InttDecode(){ };
    
    InttPacket* getNextPacket();
    
protected:

protected:
  int                packetID;
  fileEventiterator* evtItr;

  int target_lad;

  deque<int>                     vHitBuf[16];
  deque<BcoHitBuf*>              vBcoHitBuf[16];

  map<Long64_t, EventBcoHitBuf*> vEvtBcoHitBuf;

  bool endOfFile=false;
};

#endif // __INTTDECODE_H__
