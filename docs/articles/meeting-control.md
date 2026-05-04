# Meeting Control

## Meeting Status

Subscribe to `MeetingStatus` to track the current state:

| Code | State |
|---|---|
| 0 | Not in meeting |
| 1 | Connecting |
| 2 | In meeting |
| 3 | Logged out |

```csharp
sdk.MeetingStatus += (s, e) =>
    Console.WriteLine($"Meeting state: {ZrcSdkCodes.GetMeetingStatusDescription(e.ErrorCode)}");
```

## Starting and Joining Meetings

```csharp
// Start a PMI / instant meeting
sdk.StartInstantMeeting();
sdk.InstantMeetingStarted += (s, e) =>
    Console.WriteLine($"Meeting started: {e.Message}, result: {e.ErrorCode}");

// Join by meeting number
sdk.JoinMeeting("123 456 7890");

// Join via invite URL
sdk.JoinMeetingWithURL("https://zoom.us/j/1234567890?pwd=...");

// Start a scheduled meeting with host key
sdk.StartMeetingWithHostKey("123456");
```

## Ending and Leaving Meetings

```csharp
sdk.EndMeeting();    // ends for everyone (host only)
sdk.LeaveMeeting();  // leaves self, meeting continues
```

## Password-Protected Meetings

When a meeting requires a password the `MeetingNeedsPassword` event fires. `e.ErrorCode` is 1 if
the previous password attempt was wrong.

```csharp
sdk.MeetingNeedsPassword += (s, e) =>
{
    if (e.ErrorCode == 1)
        Console.WriteLine("Wrong password, try again");
    sdk.SendMeetingPassword("mypassword");
    // or: sdk.CancelEnteringMeetingPassword();
};
```

## Waiting for Host

```csharp
sdk.CancelWaitingForHost();
```

## Audio and Video

```csharp
sdk.SetAudioMute(true);   // mute
sdk.SetAudioMute(false);  // unmute
sdk.SetVideoState(true);  // start camera
sdk.SetVideoState(false); // stop camera

sdk.AudioStatus += (s, e) =>
    Console.WriteLine(e.ErrorCode == 1 ? "Muted" : "Unmuted");
```

## Cloud Recording

```csharp
sdk.StartRecording();
sdk.PauseRecording();
sdk.ResumeRecording();
sdk.StopRecording();

sdk.RecordingStatus += (s, e) =>
    Console.WriteLine(e.ErrorCode == 1 ? "Recording" : "Not recording");
```

## Participants

```csharp
sdk.ParticipantCount += (s, e) =>
    Console.WriteLine($"Participants: {e.ErrorCode}");

// Poll current count (reflects last callback value)
int count = sdk.GetParticipantCount();
```

## Zoom Room Control System (ZRCS)

```csharp
if (sdk.IsZRCSEnabled())
{
    sdk.OpenRoomControls(true);
    sdk.ControlZRCSDevice("device-id", "method-id", "param-id", "value");
    sdk.ExecuteZRCSScene("scene-id");
}
```
