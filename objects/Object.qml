import Qt 4.7

Rectangle {
    property string image: ""
    property int positionX: 0
    property int positionY: 0

    property bool player: false
    property bool massive: true
    property bool canPushOn: true
    property bool movable: false
    property bool sinks: true
    property bool liquid: false
    property bool deadly: false
    property bool exit: false

    property bool gameObject: true

    id: object
    width: engine.gridSize
    height: engine.gridSize
    x: engine.gridSize * positionX
    y: engine.gridSize * positionY
    color: Qt.rgba(0.0, 0.0, 0.0, 0.0)
    z: 2

    Image {
        source: parent.image
        anchors.fill: parent
    }

    function move(dx, dy, options) {

        var options = options || {};
        var pushObjects = options.pushObjects || false;
        var isPushed = options.isPushed || false;

        if (this.positionX + dx < 0 || this.positionY + dy < 0 ||
            this.positionX + dx >= engine.levelWidth || this.positionY + dy >= engine.levelHeight) {
            return false;
        }

        var canMove = true;
        var isExit = false;
        var isLiquid = false;
        var isDeadly = false;
        var isPlayer = false;
        var targetObjects = engine.objectsAt(this.positionX + dx, this.positionY + dy);
        for (var i = 0; i < targetObjects.length; i++) {
            var targetObject = targetObjects[i];
            if (targetObject.massive || (isPushed && !targetObject.canPushOn)) {
                if (targetObject.movable && pushObjects) {
                    if (!targetObject.move(dx, dy, { isPushed: true })) {
                        canMove = false;
                    }
                } else {
                    canMove = false;
                }
            } else {
                if (targetObject.movable && pushObjects) {
                    targetObject.move(dx, dy, { isPushed: true });
                }
            }
            if (targetObject.exit) {
                isExit = true;
            }
            if (targetObject.liquid) {
                isLiquid = true;
            }
            if (targetObject.deadly) {
                isDeadly = true;
            }
            if (targetObject.player) {
                isPlayer = true;
            }
        }

        if (!canMove) {
            return false;
        }

        this.positionX += dx;
        this.positionY += dy;

        if (isExit && this.player) {
            engine.levelFinished();
            return true;
        }

        if (isLiquid) {
            if (this.player) {
                engine.playerKilled();
                return true;
            }
            if (this.sinks) {
                engine.removeObject(this);
            }
        }

        if (isDeadly && this.player ||
            isPlayer && this.deadly) {
            engine.playerKilled();
            return true;
        }

        if (this.onIntersect) {
            this.onIntersect(targetObjects);
        }
        for (var i = 0; i < targetObjects.length; i++) {
            var targetObject = targetObjects[i];
            if (targetObject.onIntersect) {
                targetObject.onIntersect([ this ]);
            }
        }

        var sourceObjects = engine.objectsAt(this.positionX - dx, this.positionY - dy);
        for (var i = 0; i < sourceObjects.length; i++) {
            var sourceObject = sourceObjects[i];
            if (sourceObject.onIntersect) {
                var clonedObjects = [];
                for (var j = 0; j < sourceObjects.length; j++) {
                    if (j == i) {
                        continue;
                    }
                    clonedObjects.push(sourceObjects[i]);
                }
                sourceObject.onIntersect(clonedObjects);
            }
        }

        return true;
    }
}
