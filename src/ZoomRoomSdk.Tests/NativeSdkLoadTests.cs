using Xunit;
using PepperDash.ZoomRoom.Sdk;

namespace ZoomRoomSdk.Tests;

/// <summary>
/// The test machine has no libZRCSdk.so, the same state as a device where the library fails to
/// load. Construction must then throw a catchable exception instead of reaching native code.
/// </summary>
public class NativeSdkLoadTests
{
    [Fact]
    public void NativeSdkLoadError_IsSet_WhenLibraryIsMissing()
    {
        Assert.False(string.IsNullOrEmpty(ZrcSdk.NativeSdkLoadError));
    }

    [Fact]
    public void Constructor_Throws_WhenLibraryIsMissing()
    {
        var ex = Assert.Throws<DllNotFoundException>(() => new ZrcSdk());

        Assert.Contains("libZRCSdk.so", ex.Message);
    }
}
