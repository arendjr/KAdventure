import Qt 4.7

Rectangle {
    Image {
        id: background
        source: "image://resources/background.png"
        anchors.fill: parent
    }

    x: 0
    y: 0
    width: engine.levelWidth * engine.gridSize
    height: engine.levelHeight * engine.gridSize
}
