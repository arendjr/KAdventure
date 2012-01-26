import Qt 4.7

Object {
    image: "image://resources/greenball.png"

    massive: false
    canPushOn: false
    deadly: true
    z: 2

    property string direction: "right"

    function onTimeout() {

        if (this.direction == "right") {
            if (!this.move(1, 0)) {
                this.direction = "left";
                this.move(-1, 0);
            }
        } else if (this.direction == "down") {
            if (!this.move(0, 1)) {
                this.direction = "up";
                this.move(0, -1);
            }
        } else if (this.direction == "left") {
            if (!this.move(-1, 0)) {
                this.direction = "right";
                this.move(1, 0);
            }
        } else if (this.direction == "up") {
            if (!this.move(0, -1)) {
                this.direction = "down";
                this.move(0, 1);
            }
        }
    }
}
