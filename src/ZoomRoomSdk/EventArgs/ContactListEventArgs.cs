namespace PepperDash.ZoomRoom.Sdk.EventArgs;

/// <summary>
/// Distinguishes what triggered a <see cref="ContactListEventArgs"/> notification.
/// </summary>
public enum ContactListSource
{
    /// <summary>
    /// Ambient IM/presence delta (native <c>OnImUpdateContactNotification</c>) — a contact's
    /// status changed on its own and this is NOT a response to <see cref="PepperDash.ZoomRoom.Sdk.ZrcSdk.SubscribeContacts"/>.
    /// </summary>
    AmbientUpdate = 0,

    /// <summary>
    /// The actual paged directory response (native <c>OnDynamicContactListNotification</c>) to a
    /// <see cref="PepperDash.ZoomRoom.Sdk.ZrcSdk.SubscribeContacts"/> request. Only batches with this
    /// source should be used to drive phonebook paging/completion logic.
    /// </summary>
    DynamicListPage = 1,
}

/// <summary>
/// Event arguments for directory/phonebook contact list updates received from the Zoom Room.
/// Raised in response to a contact subscription (<see cref="PepperDash.ZoomRoom.Sdk.ZrcSdk.SubscribeContacts"/>)
/// as well as for ambient IM/presence updates — see <see cref="Source"/> to distinguish the two.
/// </summary>
public sealed class ContactListEventArgs : System.EventArgs
{
    /// <summary>
    /// The contact data for this update batch.
    /// </summary>
    public ContactInfo[] Contacts { get; init; } = Array.Empty<ContactInfo>();

    /// <summary>
    /// What triggered this notification. Use this to filter out ambient presence deltas when
    /// tracking paged-directory-fetch completion.
    /// </summary>
    public ContactListSource Source { get; init; }
}
