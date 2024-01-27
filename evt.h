#ifndef EVT_H
#define EVT_H

/* define data type for signal of type int16_t */
typedef unsigned int t_signal;      



/** 
  * Event base class 
  * All other events will be a subclass of this base class.
  * They would add their own additional properties and method.
  */
typedef struct sEvent
{
    t_signal sig;
}   t_event; 



enum enSignalsReserved
{
    SIG_INIT,
    SIG_ENTRY,
    SIG_EXIT,
    SIG_SHUTDOWN,
    SIG_TRIGGER_CTRL_Z,              /* This signal triggers the LED color count down */
    SIG_USER
};
#endif /* EVT_H */