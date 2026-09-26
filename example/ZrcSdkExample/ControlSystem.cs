using Crestron.SimplSharp;
using Crestron.SimplSharp.Ssh;
using Crestron.SimplSharpPro;
using PepperDash.ZoomRoom.Sdk;
using System;

namespace ZrcSdkExample;

public class ControlSystem:CrestronControlSystem
{
  private ZrcSdk? zrcSdk;

  private System.Timers.Timer startupTimer;

  public ControlSystem()
  {
    CrestronConsole.PrintLine("ZrcSdkTest Control System Started");    
  }

  public override void InitializeSystem()
  {
    CrestronConsole.PrintLine("ZrcSdkTest Control System Initializing");

    // // Fix dynamic linker symlink (run once to fix system)
    // FixDynamicLinker();

    // // Run diagnostics first to troubleshoot library loading issues
    // try
    // {
    //   CrestronConsole.PrintLine("Running library diagnostics...");
    //   LibraryDiagnostics.RunDiagnostics();

    //   //ShellExecutor.RunTests();
    // }
    // catch (Exception ex)
    // {
    //   CrestronConsole.PrintLine($"Diagnostics failed: {ex}");
    // }

    // Uncomment after library loading is working:

    startupTimer = new System.Timers.Timer(5000)
    {
      AutoReset = false
    };

    startupTimer.Elapsed += (s, e) =>
    {
      try
      {
        LoadZrcSdk();
      }
      catch (Exception ex)
      {
        CrestronConsole.PrintLine($"Initialization error: {ex}");
      }
    };
    
    startupTimer.Start();

    CrestronEnvironment.ProgramStatusEventHandler += (programEventType) =>
    {
      if (programEventType == eProgramStatusEventType.Stopping)
      {
        try
        {
          CrestronConsole.PrintLine("ZrcSdkTest stopping — disposing SDK...");
          zrcSdk?.Dispose();
          zrcSdk = null;
          CrestronConsole.PrintLine("ZrcSdkTest SDK disposed.");
        }
        catch (Exception ex)
        {
          CrestronConsole.PrintLine($"Error disposing ZRC SDK: {ex.Message}");
        }
      }
    };
  }

  private void LoadZrcSdk()
  {
    try
    {
      CrestronConsole.PrintLine("Loading ZRC SDK...");
      zrcSdk = new ZrcSdk();

      zrcSdk.Initialized += (s, e) =>
          CrestronConsole.PrintLine($"ZRC SDK initialized - Result: {e.ErrorCode}");

      // /user is a persistent writable volume — suitable for the SDK's SQLite database and logs.
      zrcSdk.Initialize("/user/zrcsdk");

      // Note: a brief Disconnected(2)→Established(0)→Connected(1) cycle during meeting start/end is normal ZRC behavior
      zrcSdk.ConnectionStateChanged += (s, e) =>
          CrestronConsole.PrintLine($"ZRC connection: {e.Message} ({ZrcSdkCodes.GetConnectionStateDescription(e.ErrorCode)})");
      zrcSdk.Error += (s, e) =>
          CrestronConsole.PrintLine($"ZR error [{ZrcSdkCodes.GetErrorDescription(e.ErrorCode)}]: {e.Message}");
      zrcSdk.PairRoomResult += (s, e) =>
      {
          string desc = ZrcSdkCodes.GetPairRoomResultDescription(e.ErrorCode);
          if (e.ErrorCode == 0)
              CrestronConsole.PrintLine($"Pair room result: {desc}");
          else
              CrestronConsole.PrintLine($"Pair room result: {desc}. Use: pairroom <activation-code>");
      };
      zrcSdk.InstantMeetingStarted += (s, e) =>
          CrestronConsole.PrintLine($"Instant meeting started — number: {e.Message}, result: {ZrcSdkCodes.GetErrorDescription(e.ErrorCode)}");

      zrcSdk.MeetingStatus += (s, e) =>
          CrestronConsole.PrintLine($"Meeting status: {ZrcSdkCodes.GetMeetingStatusDescription(e.ErrorCode)}");
      zrcSdk.StartPmiResult += (s, e) =>
          CrestronConsole.PrintLine($"PMI meeting — number: {e.Message}, result: {ZrcSdkCodes.GetErrorDescription(e.ErrorCode)}");
      zrcSdk.ExitMeeting += (s, e) =>
      {
          int result = e.ErrorCode & 0xFF;
          int reason = (e.ErrorCode >> 8) & 0xFF;
          CrestronConsole.PrintLine($"Exited meeting — result: {ZrcSdkCodes.GetErrorDescription(result)}, reason: {ZrcSdkCodes.GetExitMeetingReasonDescription(reason)}");
      };
      zrcSdk.MeetingNeedsPassword += (s, e) =>
          CrestronConsole.PrintLine($"Meeting needs password{(e.ErrorCode == 1 ? " (previous password was wrong)" : "")}");
      zrcSdk.MeetingInvite += (s, e) =>
          CrestronConsole.PrintLine($"Incoming meeting invite from: {e.CallerName} (meeting {e.MeetingNumber})");
      zrcSdk.AudioStatus += (s, e) =>
          CrestronConsole.PrintLine($"Audio mute: {(e.ErrorCode == 1 ? "muted" : "unmuted")}");
      zrcSdk.MuteOnEntry += (s, e) =>
          CrestronConsole.PrintLine($"Mute on entry: {(e.ErrorCode == 1 ? "enabled" : "disabled")}");
      zrcSdk.ParticipantCount += (s, e) =>
          CrestronConsole.PrintLine($"Participant count: {e.ErrorCode}");
      zrcSdk.HostChanged += (s, e) =>
          CrestronConsole.PrintLine($"Host changed — I am {(e.ErrorCode == 1 ? "now" : "no longer")} host");
      zrcSdk.RecordingStatus += (s, e) =>
          CrestronConsole.PrintLine($"Recording: {(e.ErrorCode == 1 ? "in progress" : "stopped")}");
      zrcSdk.ControlSystemEnabled += (s, e) =>
          CrestronConsole.PrintLine($"Control system (ZRCS): {(e.ErrorCode == 1 ? "enabled" : "disabled")}");

      // Auto-reconnect using stored credentials if available; otherwise wait for pairroom console command
      if (zrcSdk.CanRetryToPairLastRoom())
      {
          CrestronConsole.PrintLine("Stored pairing credentials found — reconnecting...");
          zrcSdk.RetryToPairRoom();
      }
      else
      {
          CrestronConsole.PrintLine("No stored pairing credentials. Use: pairroom <activation-code>");
      }

      CrestronConsole.PrintLine("✓ ZRC SDK loaded and initialized successfully");

      CrestronConsole.AddNewConsoleCommand((s) =>
      {
          if (!string.IsNullOrWhiteSpace(s))
              zrcSdk.PairRoomWithActivationCode(s.Trim());
          else
              CrestronConsole.PrintLine("Usage: pairroom <activation-code>");
      }, "pairroom", "Pair Zoom Room: pairroom <activation-code>", ConsoleAccessLevelEnum.AccessOperator);
      CrestronConsole.AddNewConsoleCommand((s) =>
      {
          if (zrcSdk.CanRetryToPairLastRoom())
              zrcSdk.RetryToPairRoom();
          else
              CrestronConsole.PrintLine("No stored pairing credentials available. Use: pairroom <activation-code>");
      }, "retrypair", "Reconnect to last paired Zoom Room using stored credentials", ConsoleAccessLevelEnum.AccessOperator);
      CrestronConsole.AddNewConsoleCommand((s) =>
      {
          if (zrcSdk.UnpairRoom())
              CrestronConsole.PrintLine("Room unpaired and credentials cleared.");
          else
              CrestronConsole.PrintLine("Unpair failed.");
      }, "unpairroom", "Unpair from current Zoom Room and clear stored credentials", ConsoleAccessLevelEnum.AccessOperator);
      CrestronConsole.AddNewConsoleCommand((s) => zrcSdk.StartInstantMeeting(), "startinstant", "Start an instant meeting", ConsoleAccessLevelEnum.AccessOperator);
      CrestronConsole.AddNewConsoleCommand((s) => zrcSdk.EndMeeting(), "endmeeting", "End the current meeting", ConsoleAccessLevelEnum.AccessOperator);
      CrestronConsole.AddNewConsoleCommand((s) => zrcSdk.WakeZoomRoomUp(), "zrwake", "Wake the Zoom Room", ConsoleAccessLevelEnum.AccessOperator);
      CrestronConsole.AddNewConsoleCommand((s) => zrcSdk.LogoutZoomRoomDevice(), "zrlogout", "Logout the Zoom Room device", ConsoleAccessLevelEnum.AccessOperator);
      CrestronConsole.AddNewConsoleCommand((s) => zrcSdk.RestartZoomRoomOS(), "zrrestart", "Restart Zoom Room OS", ConsoleAccessLevelEnum.AccessOperator);
      CrestronConsole.AddNewConsoleCommand((s) =>
      {
          if (!string.IsNullOrWhiteSpace(s))
              zrcSdk.JoinMeeting(s.Trim());
          else
              CrestronConsole.PrintLine("Usage: joinmeeting <meetingNumber>");
      }, "joinmeeting", "Join meeting by number: joinmeeting <number>", ConsoleAccessLevelEnum.AccessOperator);
      CrestronConsole.AddNewConsoleCommand((s) => zrcSdk.JoinMeetingWithURL(s), "joinurl", "Join meeting by URL", ConsoleAccessLevelEnum.AccessOperator);
      CrestronConsole.AddNewConsoleCommand((s) => zrcSdk.StartMeetingWithHostKey(s), "starthostkey", "Start meeting with host key", ConsoleAccessLevelEnum.AccessOperator);
      CrestronConsole.AddNewConsoleCommand((s) => zrcSdk.LeaveMeeting(), "leavemeeting", "Leave (not end) the current meeting", ConsoleAccessLevelEnum.AccessOperator);
      CrestronConsole.AddNewConsoleCommand((s) => zrcSdk.SendMeetingPassword(s), "meetingpw", "Send meeting password", ConsoleAccessLevelEnum.AccessOperator);
      CrestronConsole.AddNewConsoleCommand((s) => zrcSdk.StartRecording(), "recstart", "Start cloud recording", ConsoleAccessLevelEnum.AccessOperator);
      CrestronConsole.AddNewConsoleCommand((s) => zrcSdk.StopRecording(), "recstop", "Stop cloud recording", ConsoleAccessLevelEnum.AccessOperator);
      CrestronConsole.AddNewConsoleCommand((s) => zrcSdk.PauseRecording(), "recpause", "Pause cloud recording", ConsoleAccessLevelEnum.AccessOperator);
      CrestronConsole.AddNewConsoleCommand((s) => zrcSdk.ResumeRecording(), "recresume", "Resume cloud recording", ConsoleAccessLevelEnum.AccessOperator);
      CrestronConsole.AddNewConsoleCommand((s) => CrestronConsole.PrintLine($"Participant count: {zrcSdk.GetParticipantCount()}"), "partcount", "Get participant count", ConsoleAccessLevelEnum.AccessOperator);
      CrestronConsole.AddNewConsoleCommand((s) => CrestronConsole.PrintLine($"ZRCS enabled: {zrcSdk.IsZRCSEnabled()}"), "zrcsenabled", "Check if control system is enabled", ConsoleAccessLevelEnum.AccessOperator);
      CrestronConsole.AddNewConsoleCommand((s) => zrcSdk.OpenRoomControls(true), "zrcsopen", "Open room controls", ConsoleAccessLevelEnum.AccessOperator);
      CrestronConsole.AddNewConsoleCommand((s) =>
      {
          var parts = s.Split(' ');
          if (parts.Length == 4)
              zrcSdk.ControlZRCSDevice(parts[0], parts[1], parts[2], parts[3]);
          else
              CrestronConsole.PrintLine("Usage: zrcsdevice <deviceId> <methodId> <paramId> <value>");
      }, "zrcsdevice", "Control ZRCS device: zrcsdevice <deviceId> <methodId> <paramId> <value>", ConsoleAccessLevelEnum.AccessOperator);
      CrestronConsole.AddNewConsoleCommand((s) => zrcSdk.ExecuteZRCSScene(s), "zrcsscene", "Execute ZRCS scene by ID", ConsoleAccessLevelEnum.AccessOperator);
      CrestronConsole.AddNewConsoleCommand((s) => CrestronConsole.PrintLine($"Connection state: {ZrcSdkCodes.GetConnectionStateDescription(zrcSdk.GetConnectionState())}"), "zrstate", "Get SDK connection state", ConsoleAccessLevelEnum.AccessOperator);
    }
    catch (Exception ex)
    {
      CrestronConsole.PrintLine($"Error loading ZRC SDK: {ex}");
    }
  }
}
