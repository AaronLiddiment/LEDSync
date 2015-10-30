#ifndef LEDSync_h
#define LEDSync_h

#ifdef CORE_TEENSY

template<int8_t tReadyPin, int8_t tWaitPin, int8_t tID1Pin = -1, int8_t tID2Pin = -1, int8_t tID3Pin = -1, int8_t tID4Pin = -1, int8_t tID5Pin = -1> class cLEDSync
{
  private:
  	int8_t m_BoardNo;

  public:
    cLEDSync()
    {
      if (tReadyPin >= 0)
      {
        pinMode(tReadyPin, OUTPUT);
        digitalWriteFast(tReadyPin, HIGH);
      }
      if (tWaitPin >= 0)
      {
        pinMode(tWaitPin, INPUT);
        *portConfigRegister(tWaitPin) |= PORT_PCR_PE;   //pull enable
        *portConfigRegister(tWaitPin) &= ~PORT_PCR_PS;  //pull down
      }
      if (tID1Pin >= 0)
      {
        pinMode(tID1Pin, INPUT_PULLUP);
        if (tID2Pin >= 0)
        {
          pinMode(tID2Pin, INPUT_PULLUP);
          if (tID3Pin >= 0)
          {
            pinMode(tID3Pin, INPUT_PULLUP);
            if (tID4Pin >= 0)
            {
              pinMode(tID4Pin, INPUT_PULLUP);
              if (tID5Pin >= 0)
                pinMode(tID5Pin, INPUT_PULLUP);
            }
          }
        }
        delay(1);  // Digital input settling time delay
      }
      m_BoardNo = -1;
      if (tID1Pin != -1)
      {
        m_BoardNo = 0;
        if (digitalReadFast(tID1Pin) == LOW)
          m_BoardNo += 1;
        if (tID2Pin != -1)
        {
          if (digitalReadFast(tID2Pin) == LOW)
            m_BoardNo += 2;
          if (tID3Pin != -1)
          {
            if (digitalReadFast(tID3Pin) == LOW)
              m_BoardNo += 4;
            if (tID4Pin != -1)
            {
              if (digitalReadFast(tID4Pin) == LOW)
                m_BoardNo += 8;
              if (tID5Pin != -1)
              {
                if (digitalReadFast(tID5Pin) == LOW)
                  m_BoardNo += 16;
              }
            }
          }
        }
      }
    }

    void Wait()
    {
      if (tReadyPin >= 0)
        digitalWriteFast(tReadyPin, LOW);
      if (tWaitPin >= 0)
      {
        while (digitalReadFast(tWaitPin) == HIGH)
          ;
      }
    }

    void NotReady()
    {
      if (tReadyPin >= 0)
        digitalWriteFast(tReadyPin, HIGH);
    }

    void Ready()
    {
      if (tReadyPin >= 0)
        digitalWriteFast(tReadyPin, LOW);
    }

    uint8_t WaitStatus()
    {
      if (tWaitPin >= 0)
        return(digitalReadFast(tWaitPin));
      else
        return(0);
    }

    int8_t BoardNo()
    {
      return(m_BoardNo);
    }

};

#endif

#endif
