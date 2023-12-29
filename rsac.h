#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <cstring> 
/*SAC包最初是用FORTRAN语言实现的，SAC文件格式遵循FORTRAN语言的约定。每个字符串的长度为8字节(Kevnm为16字节)。
在C中重新实现SAC时，需要一个额外的字节，因为C使用‘\0’来标记字符串的终止，以避免擦除最后一个字符的内容。
在读取文件时，首先将数字部分读入头结构，然后将字符串部分读入临时缓冲区，然后将字符串从缓冲区映射到头结构。
在写文件时，先将头结构中的数值部分写入磁盘，然后将字符串从头结构映射到临时缓冲区，然后再将缓冲区写入磁盘。*/

typedef struct sac_head {
    float delta;            /* RF increment between evenly spaced samples     */
    float depmin;           /*    minimum value of dependent variable         */
    float depmax;           /*    maximum value of dependent variable         */
    float scale;            /*    amplitude scale factor (not used)           */
    float odelta;           /*    Observed increment                          */
    float b;                /* RD begining value of the independent variable  */
    float e;                /* RD ending value of the independent variable    */
    float o;                /*    event origin time(seconds wrt referece time)*/
    float a;                /*    1st arrival time (seconds wrt referece time)*/
    float internal1;        /*    internal use                                */
    float t0;               /*    user-defined time pick                      */
    float t1;               /*    user-defined time pick                      */
    float t2;               /*    user-defined time pick                      */
    float t3;               /*    user-defined time pick                      */
    float t4;               /*    user-defined time pick                      */
    float t5;               /*    user-defined time pick                      */
    float t6;               /*    user-defined time pick                      */
    float t7;               /*    user-defined time pick                      */
    float t8;               /*    user-defined time pick                      */
    float t9;               /*    user-defined time pick                      */
    float f;                /*    end time of event, sec > 0                  */
    float resp0;            /*    instrument respnse parameter (not used)     */
    float resp1;            /*    instrument respnse parameter (not used)     */
    float resp2;            /*    instrument respnse parameter (not used)     */
    float resp3;            /*    instrument respnse parameter (not used)     */
    float resp4;            /*    instrument respnse parameter (not used)     */
    float resp5;            /*    instrument respnse parameter (not used)     */
    float resp6;            /*    instrument respnse parameter (not used)     */
    float resp7;            /*    instrument respnse parameter (not used)     */
    float resp8;            /*    instrument respnse parameter (not used)     */
    float resp9;            /*    instrument respnse parameter (not used)     */
    float stla;             /*  T station latititude (degree, north positive) */
    float stlo;             /*  T station longitude (degree, east positive)   */
    float stel;             /*  T station elevation (meters, not used)        */
    float stdp;             /*  T station depth (meters, not used)            */
    float evla;             /*    event latitude (degree, north positive)     */
    float evlo;             /*    event longitude (degree, east positive)     */
    float evel;             /*    event elevation (meters, not used)          */
    float evdp;             /*    event depth (kilometer, previously meters)  */
    float mag;              /*    event magnitude                             */
    float user0;            /*    User defined variable storage area          */
    float user1;            /*    User defined variable storage area          */
    float user2;            /*    User defined variable storage area          */
    float user3;            /*    User defined variable storage area          */
    float user4;            /*    User defined variable storage area          */
    float user5;            /*    User defined variable storage area          */
    float user6;            /*    User defined variable storage area          */
    float user7;            /*    User defined variable storage area          */
    float user8;            /*    User defined variable storage area          */
    float user9;            /*    User defined variable storage area          */
    float dist;             /*    station-event distance (km)                 */
    float az;               /*    event-station azimuth                       */
    float baz;              /*    station-event azimuth                       */
    float gcarc;            /*    station-event great arc length (degrees)    */
    float internal2;        /*    internal use                                */
    float internal3;        /*    internal use                                */
    float depmen;           /*    mean value of dependent variable            */
    float cmpaz;            /*  T component azimuth (degree CW from north)    */
    float cmpinc;           /*  T component inclination (degree from vertical)*/
    float xminimum;         /*    minimum value of X (spectral files only)    */
    float xmaximum;         /*    maximum value of X (spectral files only)    */
    float yminimum;         /*    minimum value of Y (spectral files only)    */
    float ymaximun;         /*    maximum value of Y (spectral files only)    */
    float unused1;          /*    reserved for future use                     */
    float unused2;          /*    reserved for future use                     */
    float unused3;          /*    reserved for future use                     */
    float unused4;          /*    reserved for future use                     */
    float unused5;          /*    reserved for future use                     */
    float unused6;          /*    reserved for future use                     */
    float unused7;          /*    reserved for future use                     */
    int   nzyear;           /*  F GMT year corresponding to zero time of file */
    int   nzjday;           /*  F GMT julia day                               */
    int   nzhour;           /*  F GMT hour                                    */
    int   nzmin;            /*  F GMT minite                                  */
    int   nzsec;            /*  F GMT second                                  */
    int   nzmsec;           /*  F GMT millisecond                             */
    int   nvhdr;            /* R  header version number (6)                   */
    int   norid;            /*    origin ID (CSS 3.0)                         */
    int   nevid;            /*    event ID (CSS 3.0)                          */
    int   npts;             /* RF number of points per data component         */
    int   internal4;        /*    internal use                                */
    int   nwfid;            /*    waveform ID (CSS 3.0)                       */
    int   nxsize;           /*    spectral length (spectral files only)       */
    int   nysize;           /*    spectral width (spectral files only)        */
    int   unused8;          /*    reserved for future use                     */
    int   iftype;           /* RA type of file                                */
    int   idep;             /*    type of dependent variable                  */
    int   iztype;           /*    reference time equivalence                  */
    int   unused9;          /*    reserved for future use                     */
    int   iinst;            /*    type of recording instrument (not used)     */
    int   istreg;           /*    station geographic region (not used)        */
    int   ievreg;           /*    event geographic region (not used)          */
    int   ievtyp;           /*    type of event                               */
    int   iqual;            /*    quality of data (not used)                  */
    int   isynth;           /*    synthetic data flag (not used)              */
    int   imagtyp;          /*    magnitude type                              */
    int   imagsrc;          /*    source of magnitude information             */
    int   unused10;         /*    reserved for future use                     */
    int   unused11;         /*    reserved for future use                     */
    int   unused12;         /*    reserved for future use                     */
    int   unused13;         /*    reserved for future use                     */
    int   unused14;         /*    reserved for future use                     */
    int   unused15;         /*    reserved for future use                     */
    int   unused16;         /*    reserved for future use                     */
    int   unused17;         /*    reserved for future use                     */
    int   leven;            /* RA TRUE if data is evenly spaced               */
    int   lpspol;           /*    station polarity flag (left hand rule)      */
    int   lovrok;           /*    overwrite permission                        */
    int   lcalda;           /*    true if to calculate distance, azimuth      */
    int   unused18;         /*    reserved for future use                     */
    char  kstnm[9];         /*  F station name                                */
    char  kevnm[18];        /*    event name                                  */
    char  khole[9];         /*    nuclear: hole id; Other: location id;       */
    char  ko[9];            /*    event origin time id                        */
    char  ka[9];            /*    1st arrival time id                         */
    char  kt0[9];           /*    time pick 0 id                              */
    char  kt1[9];           /*    time pick 1 id                              */
    char  kt2[9];           /*    time pick 2 id                              */
    char  kt3[9];           /*    time pick 3 id                              */
    char  kt4[9];           /*    time pick 4 id                              */
    char  kt5[9];           /*    time pick 5 id                              */
    char  kt6[9];           /*    time pick 6 id                              */
    char  kt7[9];           /*    time pick 7 id                              */
    char  kt8[9];           /*    time pick 8 id                              */
    char  kt9[9];           /*    time pick 9 id                              */
    char  kf[9];            /*    end of event id                             */
    char  kuser0[9];        /*    User defined variable storage area          */
    char  kuser1[9];        /*    User defined variable storage area          */
    char  kuser2[9];        /*    User defined variable storage area          */
    char  kcmpnm[9];        /*  F channel name, three charaters               */
    char  knetwk[9];        /*    name of seismic network                     */
    char  kdatrd[9];        /*    date data was read onto computer            */
    char  kinst[9];         /*    generic name of recording instrument        */
} SACHEAD;

struct MonthDay {
    int month;
    int day;
};

struct Time {
    int days;
    int hours;
    int minutes;
    int seconds;
};

/* function */
float* read_sac(const char* name, SACHEAD* hd);
int write_sac(const char* name, SACHEAD* hd, const float* ar);
MonthDay daysToMonthDay(int year, int days);
Time secondsToDHMS(int totalSeconds);
