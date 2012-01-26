import Qt 4.7

Object {
    image: "image://resources/mine.png"

    massive: false
    canPushOn: true
    deadly: true
    z: 1

    function onIntersect(objects) {
        for (var i = 0; i < objects.length; i++) {
            engine.removeObject(objects[i]);
        }

        this.image = "qrc:/images/explosion.png";
        this.z = 4;

        timer.start();
    }

    Timer {
        id: timer
        interval: 400
        onTriggered: engine.removeObject(parent)
    }
}
