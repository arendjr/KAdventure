import Qt 4.7

Object {
    image: "image://resources/gate" + (open ? "open" : "closed") + ".png"

    massive: !open
    z: 4

    property bool open: false

    function onEventRaised(eventName, data) {
        if (eventName == "buttonPressed") {
            this.open = true;
        } else if (eventName == "buttonReleased") {
            this.open = false;
        }
    }
}
