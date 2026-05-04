using System.Runtime.InteropServices;

namespace PepperDash.ZoomRoom.Sdk;

public partial class ZrcSdk
{
    // ── P/Invoke ──────────────────────────────────────────────────────────────
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    private static extern int ZrcSdk_MuteUserAudio(IntPtr handle, int userID, int mute);
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    private static extern int ZrcSdk_MuteAllAudio(IntPtr handle, int mute);
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    private static extern int ZrcSdk_SetMuteOnEntry(IntPtr handle, int mute);
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    private static extern int ZrcSdk_AnswerUnmuteRequest(IntPtr handle, int accepted);
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    private static extern int ZrcSdk_AllowAttendeesUnmute(IntPtr handle, int allow);
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    private static extern int ZrcSdk_RequestFarEndAudioControl(IntPtr handle, int farEndUserID);
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    private static extern int ZrcSdk_GiveUpFarEndAudioControl(IntPtr handle, int farEndUserID);

    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    private static extern void ZrcSdk_SetAllowAttendeesUnmuteCallback(IntPtr handle, SdkEventCallbackDelegate? cb, IntPtr userData);
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    private static extern void ZrcSdk_SetAskUnmuteByHostCallback(IntPtr handle, SdkEventCallbackDelegate? cb, IntPtr userData);
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    private static extern void ZrcSdk_SetFEACRequestCallback(IntPtr handle, SdkEventCallbackDelegate? cb, IntPtr userData);
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    private static extern void ZrcSdk_SetFEACApprovedCallback(IntPtr handle, SdkEventCallbackDelegate? cb, IntPtr userData);
    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    private static extern void ZrcSdk_SetFEACDeclinedCallback(IntPtr handle, SdkEventCallbackDelegate? cb, IntPtr userData);

    // ── Delegates ─────────────────────────────────────────────────────────────
    private SdkEventCallbackDelegate? _audioStatusCallbackDelegate;
    private SdkEventCallbackDelegate? _muteOnEntryCallbackDelegate;
    private SdkEventCallbackDelegate? _allowAttendeesUnmuteCallbackDelegate;
    private SdkEventCallbackDelegate? _askUnmuteByHostCallbackDelegate;
    private SdkEventCallbackDelegate? _feacRequestCallbackDelegate;
    private SdkEventCallbackDelegate? _feacApprovedCallbackDelegate;
    private SdkEventCallbackDelegate? _feacDeclinedCallbackDelegate;

    // ── Events ────────────────────────────────────────────────────────────────
    /// <summary>Local audio mute state changed. <see cref="SdkEventArgs.ErrorCode"/> is 1 if muted.</summary>
    public event EventHandler<SdkEventArgs>? AudioStatus;
    /// <summary>Mute-on-entry setting changed. <see cref="SdkEventArgs.ErrorCode"/> is 1 if enabled.</summary>
    public event EventHandler<SdkEventArgs>? MuteOnEntry;
    /// <summary>Host changed whether attendees may unmute themselves. <see cref="SdkEventArgs.ErrorCode"/> is 1 if allowed.</summary>
    public event EventHandler<SdkEventArgs>? AllowAttendeesUnmuteChanged;
    /// <summary>Host asked this device to unmute. <see cref="SdkEventArgs.ErrorCode"/> is the AskUnmuteAudioByHostType enum value.</summary>
    public event EventHandler<SdkEventArgs>? AskUnmuteByHost;
    /// <summary>Far-end audio control was requested. <see cref="SdkEventArgs.ErrorCode"/> is the requester userID; Message is the display name.</summary>
    public event EventHandler<SdkEventArgs>? FEACRequest;
    /// <summary>Far-end audio control request was approved. <see cref="SdkEventArgs.ErrorCode"/> is the far-end userID; Message is the display name.</summary>
    public event EventHandler<SdkEventArgs>? FEACApproved;
    /// <summary>Far-end audio control request was declined. <see cref="SdkEventArgs.ErrorCode"/> is the far-end userID; Message is the display name.</summary>
    public event EventHandler<SdkEventArgs>? FEACDeclined;

    partial void InitializeAudioCallbacks()
    {
        _audioStatusCallbackDelegate           = OnAudioStatusCallback;
        _muteOnEntryCallbackDelegate           = OnMuteOnEntryCallback;
        _allowAttendeesUnmuteCallbackDelegate  = OnAllowAttendeesUnmuteCallback;
        _askUnmuteByHostCallbackDelegate       = OnAskUnmuteByHostCallback;
        _feacRequestCallbackDelegate           = OnFEACRequestCallback;
        _feacApprovedCallbackDelegate          = OnFEACApprovedCallback;
        _feacDeclinedCallbackDelegate          = OnFEACDeclinedCallback;

        ZrcSdk_SetAudioStatusCallback(_handle,           _audioStatusCallbackDelegate,          IntPtr.Zero);
        ZrcSdk_SetMuteOnEntryCallback(_handle,           _muteOnEntryCallbackDelegate,          IntPtr.Zero);
        ZrcSdk_SetAllowAttendeesUnmuteCallback(_handle,  _allowAttendeesUnmuteCallbackDelegate, IntPtr.Zero);
        ZrcSdk_SetAskUnmuteByHostCallback(_handle,       _askUnmuteByHostCallbackDelegate,      IntPtr.Zero);
        ZrcSdk_SetFEACRequestCallback(_handle,           _feacRequestCallbackDelegate,          IntPtr.Zero);
        ZrcSdk_SetFEACApprovedCallback(_handle,          _feacApprovedCallbackDelegate,         IntPtr.Zero);
        ZrcSdk_SetFEACDeclinedCallback(_handle,          _feacDeclinedCallbackDelegate,         IntPtr.Zero);
    }

    // ── Public API ────────────────────────────────────────────────────────────
    /// <summary>Sets the local audio mute state.</summary>
    public bool SetAudioMute(bool mute)
    {
        ThrowIfDisposed();
        return ZrcSdk_SetAudioMute(_handle, mute ? 1 : 0) == 0;
    }

    /// <summary>Mutes or unmutes a specific participant's audio. Host only.</summary>
    public bool MuteUserAudio(int userID, bool mute)
    {
        ThrowIfDisposed();
        return ZrcSdk_MuteUserAudio(_handle, userID, mute ? 1 : 0) == 0;
    }

    /// <summary>Mutes or unmutes all participants. Host only.</summary>
    public bool MuteAllAudio(bool mute)
    {
        ThrowIfDisposed();
        return ZrcSdk_MuteAllAudio(_handle, mute ? 1 : 0) == 0;
    }

    /// <summary>Enables or disables mute-on-entry for the meeting. Host only.</summary>
    public bool SetMuteOnEntry(bool mute)
    {
        ThrowIfDisposed();
        return ZrcSdk_SetMuteOnEntry(_handle, mute ? 1 : 0) == 0;
    }

    /// <summary>Accepts or declines a host request to unmute self.</summary>
    public bool AnswerUnmuteRequest(bool accepted)
    {
        ThrowIfDisposed();
        return ZrcSdk_AnswerUnmuteRequest(_handle, accepted ? 1 : 0) == 0;
    }

    /// <summary>Allows or prevents attendees from unmuting themselves. Host only.</summary>
    public bool AllowAttendeesUnmute(bool allow)
    {
        ThrowIfDisposed();
        return ZrcSdk_AllowAttendeesUnmute(_handle, allow ? 1 : 0) == 0;
    }

    /// <summary>Requests far-end audio control of another participant.</summary>
    public bool RequestFarEndAudioControl(int farEndUserID)
    {
        ThrowIfDisposed();
        return ZrcSdk_RequestFarEndAudioControl(_handle, farEndUserID) == 0;
    }

    /// <summary>Relinquishes far-end audio control of another participant.</summary>
    public bool GiveUpFarEndAudioControl(int farEndUserID)
    {
        ThrowIfDisposed();
        return ZrcSdk_GiveUpFarEndAudioControl(_handle, farEndUserID) == 0;
    }

    // ── Callbacks ─────────────────────────────────────────────────────────────
    private void OnAudioStatusCallback(string message, int isMuted, IntPtr userData) =>
        AudioStatus?.Invoke(this, new SdkEventArgs { Message = message, ErrorCode = isMuted });

    private void OnMuteOnEntryCallback(string message, int enabled, IntPtr userData) =>
        MuteOnEntry?.Invoke(this, new SdkEventArgs { Message = message, ErrorCode = enabled });

    private void OnAllowAttendeesUnmuteCallback(string message, int allow, IntPtr userData) =>
        AllowAttendeesUnmuteChanged?.Invoke(this, new SdkEventArgs { Message = message, ErrorCode = allow });

    private void OnAskUnmuteByHostCallback(string message, int type, IntPtr userData) =>
        AskUnmuteByHost?.Invoke(this, new SdkEventArgs { Message = message, ErrorCode = type });

    private void OnFEACRequestCallback(string message, int requesterUserID, IntPtr userData) =>
        FEACRequest?.Invoke(this, new SdkEventArgs { Message = message, ErrorCode = requesterUserID });

    private void OnFEACApprovedCallback(string message, int farEndUserID, IntPtr userData) =>
        FEACApproved?.Invoke(this, new SdkEventArgs { Message = message, ErrorCode = farEndUserID });

    private void OnFEACDeclinedCallback(string message, int farEndUserID, IntPtr userData) =>
        FEACDeclined?.Invoke(this, new SdkEventArgs { Message = message, ErrorCode = farEndUserID });
}
