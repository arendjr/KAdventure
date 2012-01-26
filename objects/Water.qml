import Qt 4.7

Object {
    image: "image://resources/water1.png"

    massive: false
    liquid: true
    z: 1

    function onTimeout() {
        this.image = "image://resources/water" + Math.ceil(Math.random()*3) + ".png";
    }
}
