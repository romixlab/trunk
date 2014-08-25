import QtQuick 2.0
import QtQuick.Particles 2.0

Rectangle {
    width: 360
    height: 360
    color: "gray"

    ParticleSystem {
        id: particleSystem
        anchors.fill: parent
    }

    Emitter {
        id: emitter
        anchors {
            left: parent.left
            verticalCenter: parent.verticalCenter
        }

        width: 1; height: 1
        system: particleSystem
        emitRate: 10
        lifeSpan: 6000
        lifeSpanVariation: 500
        size: 32
        endSize: 32

        velocity: AngleDirection {
            angle: -45
            angleVariation: 15
            magnitude: 100
            magnitudeVariation: 50
        }

        acceleration: AngleDirection {
            angle: 90
            magnitude: 25
        }

    }

    ImageParticle {
        source: "img/star-icon.png"
        system: particleSystem
        color: "#ffd700"
        colorVariation: 0.2
        rotation: 15
        rotationVariation: 10
        rotationVelocity: 40
        rotationVelocityVariation: 15
        entryEffect: ImageParticle.Scale
    }
}
