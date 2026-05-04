using Xunit;
using PepperDash.ZoomRoom.Sdk;

namespace ZoomRoomSdk.Tests;

public class SdkEventArgsTests
{
    [Fact]
    public void SdkEventArgs_DefaultValues_AreEmpty()
    {
        var args = new SdkEventArgs();

        Assert.Equal(string.Empty, args.Message);
        Assert.Equal(0, args.ErrorCode);
    }

    [Fact]
    public void SdkEventArgs_PropertiesRoundtrip()
    {
        var args = new SdkEventArgs
        {
            Message   = "SDK initialized",
            ErrorCode = 42
        };

        Assert.Equal("SDK initialized", args.Message);
        Assert.Equal(42, args.ErrorCode);
    }

    [Fact]
    public void SdkEventArgs_Message_CanBeSetToNull()
    {
        var args = new SdkEventArgs();
        args.Message = null!;

        Assert.Null(args.Message);
    }

    [Fact]
    public void SdkEventArgs_ErrorCode_NegativeValue_Roundtrips()
    {
        var args = new SdkEventArgs { ErrorCode = -1 };
        Assert.Equal(-1, args.ErrorCode);
    }
}
