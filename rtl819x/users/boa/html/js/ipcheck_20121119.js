
function checkIP ( IPvalue ) 
{
  var errorString = "";
  theName = "IP Address: ";

  var ipPattern = /^(\d{1,3})\.(\d{1,3})\.(\d{1,3})\.(\d{1,3})$/;
  var ipArray = IPvalue.match(ipPattern); 

  if (IPvalue == "0.0.0.0")
  {
    errorString = theName + IPvalue+' is a special IP address and cannot be used here.';
  }
  else 
    if (IPvalue == "255.255.255.255")
    {
      errorString = theName + IPvalue+' is a special IP address and cannot be used here.';
    }
    else
      if (ipArray == null)
      {
        errorString = theName + IPvalue+' is not a valid IP address.';
      }
      else 
        for (i = 1; i < 5; i++)
        {
          thisSegment = ipArray[i];
          if (thisSegment > 255) 
          {
            errorString = theName + IPvalue+' is not a valid IP address.';
            i = 4;
          }
        }

  if (errorString == "")
  {
    return true;
  }
  else
  {
    alert (errorString);
    return false;
  }
  return true;
}

function checkLanIP ( IPvalue )
{
  var errorString = "";
  theName = "IP Address: ";

  var ipPattern = /^(\d{1,3})\.(\d{1,3})\.(\d{1,3})\.(\d{1,3})$/;
  var ipArray = IPvalue.match(ipPattern);

  if (IPvalue == "0.0.0.0")
  {
    errorString = theName + IPvalue+' is a special IP address and cannot be used here.';
  }
  else
    if (IPvalue == "255.255.255.255")
    {
      errorString = theName + IPvalue+' is a special IP address and cannot be used here.';
    }
    else
      if (ipArray == null)
      {
        errorString = theName + IPvalue+' is not a valid IP address.';
      }
      else
      {
        for (i = 1; i < 5; i++)
        {
          thisSegment = ipArray[i];
          if (thisSegment > 255)
          {
            errorString = theName + IPvalue+' is not a valid IP address.';
            i = 4;
          }
        }
        thisSegment = ipArray[4];
        if (thisSegment == 255)
        {
            errorString = theName + IPvalue+' is a special IP address and cannot be used here.';            
        }
      } 

  if (errorString == "")
  {
    return true;
  }
  else
  {
    alert (errorString);
    return false;
  }
  return true;
}

function checkGwIP ( IPvalue )
{
  var errorString = "";
  theName = "Gateway IP Address: ";

  var ipPattern = /^(\d{1,3})\.(\d{1,3})\.(\d{1,3})\.(\d{1,3})$/;
  var ipArray = IPvalue.match(ipPattern);

  if (IPvalue == "0.0.0.0")
  {
    errorString = theName + IPvalue+' is a special IP address and cannot be used here.';
  }
  else
    if (IPvalue == "255.255.255.255")
    {
      errorString = theName + IPvalue+' is a special IP address and cannot be used here.';
    }
    else
      if (ipArray == null)
      {
        errorString = theName + IPvalue+' is not a valid IP address.';
      }
      else
        for (i = 1; i < 5; i++)
        {
          thisSegment = ipArray[i];
          if (thisSegment > 255)
          {
            errorString = theName + IPvalue+' is not a valid IP address.';
            i = 4;
          }
        }

  if (errorString == "")
  {
    return true;
  }
  else
  {
    alert (errorString);
    return false;
  }
  return true;
}

function checkNetMask( value )
{
  var masktmpvalue  = value.split(".");
  var Sub_Mask = 0;
  var len = masktmpvalue.length;
  if(len != 4) {
      alert("Invalid subnet mask!\n");
      return false;
  }
  var i;
  for( i=0; i<len; i++ ) {
      if((masktmpvalue[i] >= 0) && (masktmpvalue[i] <= 255))
          Sub_Mask = ((Sub_Mask<<8) | parseInt(masktmpvalue[i]));
      else {
          alert("Invalid subnet mask!\n");
          return false;
      }
  }
  var mask_str = (~Sub_Mask).toString(2);
  var mask_len = mask_str.length;
  var bZero = false;
  for( i=mask_len-1; i>=0; i-- )
  {
     if( mask_str.charAt(i)=="0" )
     {
        bZero = true;
        continue;
     };

     if( (bZero==true) && (mask_str.charAt(i)=="1") )
     {
        alert("Invalid subnet mask: "+value+".\n");
        return false;
     }
  }
  return true;
} 

function checkDHCPS( lan, start, end, mask )
{
  if( !checkIP(lan) || !checkIP(start) || !checkIP(end) || !checkNetMask(mask) )
      return false;

  var starttmpvalue = start.split(".");
  var endtmpvalue   = end.split(".");
  var masktmpvalue  = mask.split(".");
  var tmp           = lan.split(".");
  var len = tmp.length;
  var i, Lan_IP, Start_IP, End_IP, Sub_Mask;
  for(i=0; i<len; i++) {
        Lan_IP   = ( (Lan_IP<<8)   | parseInt(tmp[i]) );
        Start_IP = ( (Start_IP<<8) | parseInt(starttmpvalue[i]) );
        End_IP   = ( (End_IP<<8)   | parseInt(endtmpvalue[i]) );
        Sub_Mask = ( (Sub_Mask<<8) | parseInt(masktmpvalue[i]) );
  };

  if ( (Start_IP-End_IP)>0 )
  {
      alert("Start IP address must be less than End IP address.\n");
      return false;
  }

  var net_IP = Lan_IP & Sub_Mask;
  if ( net_IP != (Start_IP&Sub_Mask) ) {
      alert("Start IP and LAN IP are not in the same subnet.\n");
      return false;
  }

  if ( net_IP != (End_IP&Sub_Mask) ) {
      alert("End IP and LAN IP are not in the same subnet.\n");
      return false;
  }

  if ( (Start_IP==net_IP) || (End_IP==net_IP) ) {
      alert("Network address cannot be used as start or end IP address.\n");
      return false;
  }

  var bcast_IP = net_IP+(~Sub_Mask);
  if ( (Start_IP == bcast_IP) || (End_IP == bcast_IP) ) {
      alert("Broadcast address cannot be used as start or end IP address.\n");
      return false;
  }

  if ( (Start_IP == Lan_IP) || (End_IP == Lan_IP) ) {
      alert("LAN IP address of the router cannot be used as start or end IP address.\n");
      return false;
  }
  return true;
}
