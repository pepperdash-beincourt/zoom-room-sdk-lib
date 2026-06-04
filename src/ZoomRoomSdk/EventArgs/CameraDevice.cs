namespace PepperDash.ZoomRoom.Sdk.EventArgs;

/// <summary>
/// Managed representation of a Zoom Room camera device (a flattened subset of the SDK
/// <c>Device</c> struct). Used to enumerate the room's local cameras and identify the
/// active one. Select a camera with <see cref="ZrcSdk.SetCurrentCamera"/> using <see cref="Id"/>.
/// </summary>
public sealed class CameraDevice
{
    /// <summary>Stable device ID used to select this camera.</summary>
    public string Id          { get; init; } = string.Empty;
    /// <summary>Device name reported by the SDK.</summary>
    public string Name        { get; init; } = string.Empty;
    /// <summary>UI-friendly display name (falls back to <see cref="Name"/> when blank).</summary>
    public string DisplayName { get; init; } = string.Empty;
    /// <summary>True when this is the currently selected/active camera.</summary>
    public bool   IsSelected  { get; init; }
}
