using System.Runtime.InteropServices;

namespace PepperDash.ZoomRoom.Sdk;

public partial class ZrcSdk
{
    [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)]
    private struct ZrcSIPCallNative
    {
        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 128)] public string callID;
        public int status;
        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 256)] public string peerDisplayName;
        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 128)] public string peerNumber;
        public int isIncomingCall;
        public long elapsedCallTime;
    }

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    private delegate void ZrcSIPCallCallbackDelegate(IntPtr callPtr, IntPtr userData);

    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
    private static extern int ZrcSdk_DeclineSIPCall(IntPtr handle, string callID);
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
    private static extern int ZrcSdk_CallSIP(IntPtr handle, string uri);
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
    private static extern int ZrcSdk_SendDTMFToSIPCall(IntPtr handle, string dtmf, string? callID);
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
    private static extern int ZrcSdk_TerminateSIPCall(IntPtr handle, string callID);
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
    private static extern int ZrcSdk_HoldSIPCall(IntPtr handle, string callID);
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
    private static extern int ZrcSdk_UnholdSIPCall(IntPtr handle, string callID);
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
    private static extern int ZrcSdk_CallOutPSTNUser(IntPtr handle, string phoneNumber, int cancelCall, int hasVoicePrompt);
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    private static extern void ZrcSdk_SetSIPCallStatusCallback(IntPtr handle, ZrcSIPCallCallbackDelegate? cb, IntPtr userData);
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    private static extern void ZrcSdk_SetSIPServiceStatusCallback(IntPtr handle, SdkEventCallbackDelegate? cb, IntPtr userData);

    private ZrcSIPCallCallbackDelegate? _sipCallStatusCallbackDelegate;
    private SdkEventCallbackDelegate?   _sipServiceStatusCallbackDelegate;

    /// <summary>SIP call status changed (incoming, active, terminated, etc.).</summary>
    public event EventHandler<SIPCall>? SIPCallStatus;
    /// <summary>SIP service registration status changed. <see cref="SdkEventArgs.ErrorCode"/> is the status enum; Message is the display name.</summary>
    public event EventHandler<SdkEventArgs>? SIPServiceStatus;

    partial void InitializePhoneCallbacks()
    {
        _sipCallStatusCallbackDelegate    = OnSIPCallStatusCallback;
        _sipServiceStatusCallbackDelegate = OnSIPServiceStatusCallback;
        ZrcSdk_SetSIPCallStatusCallback(_handle,    _sipCallStatusCallbackDelegate,    IntPtr.Zero);
        ZrcSdk_SetSIPServiceStatusCallback(_handle, _sipServiceStatusCallbackDelegate, IntPtr.Zero);
    }

    /// <summary>Places an outbound SIP call to the given URI / number.</summary>
    public bool CallSIP(string uri) { ThrowIfDisposed(); return ZrcSdk_CallSIP(_handle, uri) == 0; }

    /// <summary>
    /// Sends DTMF digits to an active SIP call. Pass the call's <c>callID</c>, or null/empty to
    /// target the single active call.
    /// </summary>
    public bool SendDTMFToSIPCall(string dtmf, string? callID = null) { ThrowIfDisposed(); return ZrcSdk_SendDTMFToSIPCall(_handle, dtmf, callID) == 0; }

    /// <summary>Declines an incoming SIP call. Pass the call's <c>callID</c> (or null/empty for the single active call).</summary>
    public bool DeclineSIPCall(string callID) { ThrowIfDisposed(); return ZrcSdk_DeclineSIPCall(_handle, callID) == 0; }

    /// <summary>Terminates an active SIP call.</summary>
    public bool TerminateSIPCall(string callID) { ThrowIfDisposed(); return ZrcSdk_TerminateSIPCall(_handle, callID) == 0; }

    /// <summary>Holds an active SIP call.</summary>
    public bool HoldSIPCall(string callID) { ThrowIfDisposed(); return ZrcSdk_HoldSIPCall(_handle, callID) == 0; }

    /// <summary>Resumes a held SIP call.</summary>
    public bool UnholdSIPCall(string callID) { ThrowIfDisposed(); return ZrcSdk_UnholdSIPCall(_handle, callID) == 0; }

    /// <summary>
    /// Dials out a PSTN phone number into the current meeting via the third-party meeting helper.
    /// <paramref name="cancelCall"/> cancels an in-progress call-out; <paramref name="hasVoicePrompt"/>
    /// rings the call on the Zoom Room.
    /// </summary>
    public bool CallOutPSTNUser(string phoneNumber, bool cancelCall = false, bool hasVoicePrompt = false)
    {
        ThrowIfDisposed();
        return ZrcSdk_CallOutPSTNUser(_handle, phoneNumber, cancelCall ? 1 : 0, hasVoicePrompt ? 1 : 0) == 0;
    }

    private void OnSIPCallStatusCallback(IntPtr callPtr, IntPtr userData)
    {
        if (callPtr == IntPtr.Zero) return;
        var n = Marshal.PtrToStructure<ZrcSIPCallNative>(callPtr);
        SIPCallStatus?.Invoke(this, new SIPCall
        {
            CallID          = n.callID ?? string.Empty,
            Status          = n.status,
            PeerDisplayName = n.peerDisplayName ?? string.Empty,
            PeerNumber      = n.peerNumber ?? string.Empty,
            IsIncomingCall  = n.isIncomingCall != 0,
            ElapsedCallTime = n.elapsedCallTime,
        });
    }

    private void OnSIPServiceStatusCallback(string message, int status, IntPtr userData) =>
        SIPServiceStatus?.Invoke(this, new SdkEventArgs { Message = message, ErrorCode = status });
}

/// <summary>Represents the current state of a SIP phone call.</summary>
public class SIPCall
{
    /// <summary>Unique call identifier assigned by the SIP stack.</summary>
    public string CallID          { get; set; } = string.Empty;
    /// <summary>Current SIP call status code.</summary>
    public int    Status          { get; set; }
    /// <summary>Display name of the remote party.</summary>
    public string PeerDisplayName { get; set; } = string.Empty;
    /// <summary>Phone number or SIP URI of the remote party.</summary>
    public string PeerNumber      { get; set; } = string.Empty;
    /// <summary><see langword="true"/> if this is an inbound (incoming) call.</summary>
    public bool   IsIncomingCall  { get; set; }
    /// <summary>Elapsed call duration in seconds since the call was answered.</summary>
    public long   ElapsedCallTime { get; set; }
}
