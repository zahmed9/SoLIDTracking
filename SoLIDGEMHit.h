#ifndef ROOT_SoLID_GEM_Hit
#define ROOT_SoLID_GEM_Hit
//***********************************************************************
//*
//*File containing all the GEM hit related classes
//*Hit: generic 1D hit class
//*SoLIDRawHit: 1D clustered hit from each readout plane
//*SoLIDMCRawHit: the MC version of SoLIDRawHit, contains extra MC info
//*SoLIDHit: 2D hit on each GEMTracker, after amplitude matching
//*SoLIDMCHit: MC version of SoLIDHit, contains extra MC info
//*
//***********************************************************************
//Hall A Analyzer
#ifdef MCDATA
#include "SimDecoder.h"  // for MC data support
#endif
//SoLIDTracking
#include "SoLIDGEMReadOut.h"
#include "SoLIDGEMChamber.h"

class SoLIDGEMReadOut;
//-------------------------------   Hit   ------------------------------//
 class Hit : public TObject {

  public:
    Hit() : fReadOut(0) {}
    Hit( Double_t pos, Double_t res, SoLIDGEMReadOut* readout )
      : fPos(pos), fResolution(res), fReadOut(readout) { assert(fReadOut); }
    // Default copy and assignment are fine
    //    Hit( const Hit& );
    //    Hit& operator=( const Hit& );
    virtual ~Hit() {}

    virtual Int_t Compare( const TObject* obj ) const;
    virtual Int_t Compare( const Hit* rhs, Double_t maxdist ) const;
    virtual Bool_t IsSortable () const { return kTRUE; }
    virtual void Print( Option_t* opt="" ) const;

    Double_t GetPos()        const { return fPos; }
    Double_t GetResolution() const { return fResolution; }

    SoLIDGEMReadOut*   GetReadOut()      const { return fReadOut; }

  protected:
    Double_t fPos;             // Hit position along plane coordinate axis (m)
    Double_t fResolution;      // Resolution of fPos (sigma, m)
    SoLIDGEMReadOut* fReadOut; //! Pointer to the readout obj where this hit occurred

    ClassDef(Hit,1)        // Generic tracker plane hit
  };
//--------------------------------------------------------------------//
//--------------------------- SoLIDRawHit ----------------------------//
  class SoLIDRawHit : public Hit {

  public:
    SoLIDRawHit() {}
    SoLIDRawHit( Double_t pos, Double_t adc_sum, UInt_t num_strips, Int_t type,
            Double_t res, SoLIDGEMReadOut* readout ) :
      Hit(pos, res, static_cast<SoLIDGEMReadOut*>(readout)), fADCsum(adc_sum),
      fSize(num_strips), fType(type) {}
    virtual ~SoLIDRawHit() {}

    virtual void Print( Option_t* opt="" ) const;

    Double_t GetADCsum()     const { return fADCsum; }
    UInt_t   GetSize()       const { return fSize; }
    Int_t    GetType()       const { return fType; }

  protected:
    Double_t fADCsum;      // Sum of ADC values of active strips
    UInt_t   fSize;        // Number of active strips
    Int_t    fType;        // Result code of cluster analysis

    ClassDef(SoLIDRawHit,1)     // Hit on an ADC-based readout plane, e.g. GEMs
  };

#ifdef MCDATA
  //-------------------------------------------------------------------------//
  //--------------------------- SoLIDMCRawHit -------------------------------//
  // Monte Carlo hit class. Same as a hit plus the MC truth info.

  class SoLIDMCRawHit : public SoLIDRawHit, public Podd::MCHitInfo {

  public:
    SoLIDMCRawHit() {}
    SoLIDMCRawHit( Double_t pos, Double_t adc_sum, UInt_t num_strips, Int_t type,
              Double_t res, SoLIDGEMReadOut* readout, Int_t mctrk, Double_t mcpos,
              Double_t mctime, Int_t num_bg_strips )
      : SoLIDRawHit(pos, adc_sum, num_strips, type, res, readout),
        MCHitInfo(mctrk, mcpos, mctime, num_bg_strips) {}
    virtual ~SoLIDMCRawHit() {}

    virtual void Print( Option_t* opt="" ) const;

    ClassDef(SoLIDMCRawHit,2)   // Monte Carlo hit in ADC-based readout plane
  };
#endif // MCDATA

//------------------------------ SoLIDGEMHit ---------------------------------//
//a pair of hits, one from u and the other from v readout plane
  class SoLIDGEMHit : public TObject {
    public:
    SoLIDGEMHit(Int_t chamberID, Int_t trackerID, Double_t r, Double_t phi, Double_t z, Hit* uhit, Hit* vhit);
    ~SoLIDGEMHit(){};
    
    Int_t GetChamberID() const { return fChamberID; }
    Int_t GetTrackerID() const { return fTrackerID; }
    Double_t GetZ() const { return fZ; }
    Double_t GetX() const { return fX; }
    Double_t GetY() const { return fY; }
    Double_t GetR() const { return fR; }
    Double_t GetPhi() const { return fPhi; }
    Double_t GetQU() const { return dynamic_cast<SoLIDRawHit*>(fUHit)->GetADCsum(); }
    Double_t GetQV() const { return dynamic_cast<SoLIDRawHit*>(fVHit)->GetADCsum(); }
    Hit * GetUHit() const { return fUHit; }
    Hit * GetVHit() const { return fVHit; }
    
    protected:
    Int_t    fChamberID;
    Int_t    fTrackerID;
    Double_t fX;
    Double_t fY;
    Double_t fR;
    Double_t fPhi;
    Double_t fZ;
    Hit *fUHit;
    Hit *fVHit;
    
    ClassDef(SoLIDGEMHit, 1)
  };
  
#ifdef MCDATA
  class SoLIDMCGEMHit : public SoLIDGEMHit {
    public:
    SoLIDMCGEMHit(Int_t chamberID, Int_t trackerID, Double_t r, Double_t phi, Double_t z, Hit* uhit, Hit* vhit) :
    SoLIDGEMHit(chamberID, trackerID, r, phi, z, uhit, vhit) {};
    ~SoLIDMCGEMHit(){};
    
    Int_t IsSignalHit() {
      if (dynamic_cast<SoLIDMCRawHit*>(fUHit)->fMCTrack == 1 && 
          dynamic_cast<SoLIDMCRawHit*>(fVHit)->fMCTrack == 1) return 1;
      else return 0;
    }
    
    ClassDef(SoLIDMCGEMHit, 1)
  };
#endif
//----------------------------------------------------------------------------//

#endif



























