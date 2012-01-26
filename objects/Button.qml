import Qt 4.7

Object {
    image: "image://resources/button.png"

    massive: false

    function onIntersect(objects) {
        if (objects.length > 0) {
            engine.eventRaised("buttonPressed");
        } else {
            engine.eventRaised("buttonReleased");
        }
    }
}
