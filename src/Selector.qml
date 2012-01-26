import Qt 4.7

Rectangle {
    property int positionX: 0
    property int positionY: 0

    id: selector
    width: engine.gridSize
    height: engine.gridSize
    x: engine.gridSize * positionX
    y: engine.gridSize * positionY
    color: Qt.rgba(0.0, 0.0, 0.0, 0.0)
    z: 10

    Image {
        source: "image://resources/selector.png"
        anchors.fill: parent
    }
}
