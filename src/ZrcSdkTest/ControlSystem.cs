using Crestron.SimplSharp;
using Crestron.SimplSharpPro;
using System;

namespace ZrcSdkTest;

public class ControlSystem:CrestronControlSystem
{
  private const string activationCode = "2132-4857-9379-8334";

  private ZrcSdk zrcSdk;

  public ControlSystem()
  {
    CrestronConsole.PrintLine("ZrcSdkTest Control System Started");

  }
  
  public override void InitializeSystem()
  {
    CrestronConsole.PrintLine("ZrcSdkTest Control System Initializing");

    // Run diagnostics first to troubleshoot library loading issues
    try
    {
      CrestronConsole.PrintLine("Running library diagnostics...");
      LibraryDiagnostics.RunDiagnostics();
    }
    catch (Exception ex)
    {
      CrestronConsole.PrintLine($"Diagnostics failed: {ex}");
    }

    // Uncomment after library loading is working:
    /*
    zrcSdk = new ZrcSdk();
    zrcSdk.Initialize(string.Empty);
    zrcSdk.PairRoomWithActivationCode(activationCode);
    */
    
    CrestronConsole.PrintLine("ZrcSdkTest Control System Initialized");
  }
}
