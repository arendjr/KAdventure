import Qt 4.7

Object {
    image: "image://resources/transporterdown.png"

    massive: false
    z: 1

    function onIntersect(objects) {
        for (var i = 0; i < objects.length; i++) {
            objects[i].move(0, 1);
        }
    }
}
