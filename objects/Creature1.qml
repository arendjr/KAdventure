import Qt 4.7

Object {
    image: "image://resources/creature1" + direction + ".png"

    massive: false
    canPushOn: false
    deadly: true
    z: 2

    property string direction: "left"

    function onTimeout() {

        if (this.direction == "right") {
            if (this.move(0, 1)) {
                this.direction = "down";
            } else {
                if (!this.move(1, 0)) {
                    this.direction = "up";
                }
            }
        } else if (this.direction == "down") {
            if (this.move(-1, 0)) {
                this.direction = "left";
            } else {
                if (!this.move(0, 1)) {
                    this.direction = "right";
                }
            }
        } else if (this.direction == "left") {
            if (this.move(0, -1)) {
                this.direction = "up";
            } else {
                if (!this.move(-1, 0)) {
                    this.direction = "down";
                }
            }
        } else if (this.direction == "up") {
            if (this.move(1, 0)) {
                this.direction = "right";
            } else {
                if (!this.move(0, -1)) {
                    this.direction = "left";
                }
            }
        }
    }
}
