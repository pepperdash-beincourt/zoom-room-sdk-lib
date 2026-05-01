# Pairing a Room

A Zoom Room must be paired before the SDK can control it. Pairing links this controller to a
specific Zoom Room via the Zoom admin portal.

## First-Time Pairing

1. In the Zoom admin portal, navigate to **Room Management → Zoom Rooms → Add Room → Activation Code**.
2. Pass the code to the SDK:

```csharp
sdk.PairRoomResult += (s, e) =>
{
    if (e.ErrorCode == 0)
        Console.WriteLine("Paired successfully");
    else
        Console.WriteLine($"Pair failed: {ZrcSdkCodes.GetPairRoomResultDescription(e.ErrorCode)}");
};

sdk.PairRoomWithActivationCode("XXXXXX");
```

The `PairRoomResult` event fires asynchronously when the operation completes.

## Reconnect on Restart

After a successful pairing the SDK stores encrypted credentials in the `configPath` directory
(e.g. `/user/zrcsdk/ZRCSDK.conf`). On subsequent boots you can reconnect without a new code:

```csharp
if (sdk.CanRetryToPairLastRoom())
    sdk.RetryToPairRoom();
else
    // prompt operator for activation code
```

> **Important:** Always call `sdk.Dispose()` before the process exits. This flushes the credential
> file to disk. If the process is killed before `IZRCSDK::DestroyInstance()` is called, the token
> may not be persisted and the next `RetryToPairRoom()` will fail with error **5002**.

## Unpairing

```csharp
sdk.UnpairRoom();
```

This clears all stored credentials. The next boot will require a new activation code.

## Common Pair Result Codes

| Code | Meaning |
|---|---|
| 0 | Success |
| 100 | Invalid activation code |
| 101 | Activation code already used |
| 102 | Activation code expired |
| 5002 | Authentication token invalid — credentials not flushed on last shutdown |
| 30055016 | Room already associated with another device |
