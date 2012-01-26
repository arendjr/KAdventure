import Qt 4.7

Object {
    image: "image://resources/raft.png"

    massive: false
    canPushOn: false
    movable: true
    sinks: false

    function onIntersect(objects) {
        for (var i = 0; i < objects.length; i++) {
            var object = objects[i];
            if (object.liquid) {
                object.liquid = false;
                this.canPushOn = true;
                this.movable = false;
            }
        }
    }
}
