import QtQuick 2.3
import QtQuick.Window 2.2

Window {
    id: root
    visible: true
    minimumWidth: 640
    minimumHeight: 480

    function startup() {
        accounts.update();
    }

    Accounts {
        id: accounts
    }

    Account {
        id: account
        visible: false
    }

    Component.onCompleted: {
        console.log("Connecting listener of password requests")
        app.requestPassword.connect(password_request.show)
        password_request.done.connect(app.responsePassword)

        accounts.accountSelected.connect(account.show)

        app.postinitDone.connect(root.startup)
    }

    ContextMenu {
        id: context_menu
    }

    PasswordRequest {
        id: password_request
    }
}
