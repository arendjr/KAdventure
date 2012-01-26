import Qt 4.7

Object {
    image: "image://resources/player.png"
    player: true
    massive: false
    z: 3

    function onPlayerMoved(dx, dy) {
        this.move(dx, dy, { pushObjects: true });
    }
}
