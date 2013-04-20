import bb.cascades 1.0

Page {
    titleBar: TitleBar {
        title: "Permissions Test"
    }
    ListView {
        id: permissionsList
        objectName: "permissionsList"
        listItemComponents: [
            ListItemComponent {
                StandardListItem {
                    title: ListItemData.label
                    description: ListItemData.name
                    imageSource: ListItemData.enabled ? "asset:///images/item-yes.png" : "asset:///images/item-no.png"
                }
            }
        ]
    }
}
