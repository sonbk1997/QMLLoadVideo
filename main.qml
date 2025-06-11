import QtQuick 2.14
import QtQuick.Window 2.14

Window {
    visible: true
    width: 640
    height: 480
    title: qsTr("Hello World")
    color: "black"

    Image {
        id: image
        anchors.fill: parent
        source: "image://imageprovider"
        cache: false
        asynchronous: false
        function reload() {
            var oldSource = source;
            source = "";
            source = oldSource;
        }
    }

    Connections {
        target: imageprovider
        onUpdateGraphicsImage: image.reload();
    }
}
