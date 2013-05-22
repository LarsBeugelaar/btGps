
// currently only the first 7 parts of GPGGA are supported, with support for returning structs containing longitute and magnitude in decimal notation

// literature:
// http://www.winsystems.com/software/nmea.pdf
// http://aprs.gids.nl/nmea/
// http://en.wikipedia.org/wiki/NMEA_0183
// http://nl.wikipedia.org/wiki/NMEA-0183
// https://play.google.com/store/apps/details?id=com.Saenko.GpsOverBt


class gpsOverBluetooth {
  // variables
  public:
  // input buffer
  char GPRMC[80];
  
  // GPRMC data

  char hours, minutes, seconds;
  char receiverStatus;  
  
  char latitudeDegrees;
  float latitudeSeconds;
  char latitudeOrientation;
  char longitudeDegrees;
  float longitudeSeconds;
  char longitudeOrientation;
  
  char orientation;
  float speedKnots;
  float trackMadeGood;
  char day, month, year;
  float mageticVariation;
  char variationDirection;
  
  // GPRMC extensions
  float decimalLongitude, decimalLatitude;
  float speedKilometer;
  
  private:
  char identifier[5]; // holds the talker id and identifier
  char sentenceType; // holds the type of sentence that is received at that moment 
  int count; // keep track of the number of bytes received after the dollarsign (begin sentence)
  int commaCount; // keeps track of the number of comma's we had in the sentence
  int charCount; // keeps track of the number of chars we have had after a comma
  
  char dataInput; // holds the current byte of data input
  
  public:
  // constructor
  gpsOverBluetooth();
  
  // used to give bytes of data to bluetooth object
  uint8_t Input(uint8_t);
  
  private:
  // used to determine which NMEA sentence is beeïng received
  byte determineSentenceType();
  // looks wich of the supported NMEA sentences is received and processes the data accordingly
  void writeSentence();
  // function that is called when GPGGA sentence data is received.
  void writeSentenceGPGGA();
  
  // returns float with decimal longitude
  float decimalLongitude();
  // returns float with decimal latitude
  float decimalLatitude();
};

gpsOverBluetooth :: gpsOverBluetooth()
{
  //  nothing will do anything with these values
  commaCount = 65535;
  count = 65535;
  charCount = 65535;
}

uint8_t gpsOverBluetooth :: Input(uint8_t data)
{
  // if the '$' is received a new sentence has started, so count = 0;
  if (data == '$')
  {
    count = 0;
    commaCount = 0;
    charCount = 0;
  }
  else if (data == 13)
  {
    // carriage return, end of sentence
    count = 65535;
    commaCount = 65535;
    charCount = 65535;
  }
  else if (data == 44) commaCount++; // comma, next data of sentence
  else charCount++;   // nothing of the above, so problably just data, so move to next character
  
  if (count <5 )
  {
    // fill identifier
    identifier[count] = data;
  }
  else if (count == 5)
  {
    // fill identifier and determine type of sentence
    identifier[count] = data;
    sentenceType = determineSentenceType();
  }
  else
  {
    // else it 
    writeSentence();
  }
  count++;
};

void gpsOverBluetooth :: writeSentence()
{
  switch(sentenceType)
  {
    case 0:
    // error
    break;
    case 1:
    // GPGGA - Global Positioning System Fix Data
    break;
    case 2:
    // GPGLL - Geographic position, latitude / longitude
    break;
    case 3:
    // GPGSA - GPS DOP and active satellites
    break;
    case 4:
    // GPGSV - Detailed satellite data
    break;
    case 5:
    // GPRMC - Recommended Minimum Specific GPS/TRANSIT Data
    writeSentenceGPRMC();
    break;
    case 6:
    // GPVTG - Track made good and ground speed
    break;
    case 7:
    // GPZDA - Date and Time
    break;
    default:
    // should not get here
    break;
  }
}
  
  void gpsOverBluetooth :: writeSentenceGPRMC()
  {
    switch(commaCount)
    {
      case 0:
      strTime[charCount] = dataInput;
      break;
      case 1:
      strLatitude[charCount] = dataInput;
      break;
      case 2:
      strLatitudeMeridian = dataInput;
      break;
      case 3:
      strLongitude[charCount] = dataInput;
      break;
      case 4:
      strLongitudemeridian = dataInput;
      break;
      case 5:
      strFixQuality = dataInput;
      break;
      case 6:
      strFatelliteCount[charCount] = dataInput;
      break;
      case 7:
      strDilutionOfPrecision[charCount] = dataInput;
      break;
      default:
      // Don't care much
      break;
    }
    
  }
  
  float gpsOverBluetooth :: decimalLongitude()
  {
    char temp[2] = { strLongitude[0], strlongitude[1] };
    decimalLongitude = atoi(temp);
    
    
    
    
  }
