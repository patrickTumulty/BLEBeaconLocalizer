
import { clearChildElements } from "./utils.js";
import { WSProtocol } from "./messaging.js"

class MessageType {

    constructor(name, id) {
        this.name = name;
        this.id = id;
    }

    toString() {
        return `[${this.id}:${this.name}]`;
    }
}

export function registerMac() {
    const macInput = document.getElementById("mac")
    console.log("Registering Beacom MAC: " + macInput.value)
    let buffer = WSProtocol.createRegisterBeaconPacket(macInput.value)
    WSProtocol.send(ws, buffer)
}

window.registerMac = registerMac;

const REPORT_ANCHORS = new MessageType("Report Anchors", 1)
const REPORT_ANCHOR_TELEMETRY = new MessageType("Report Anchor Telemetry", 2)
const REPORT_TAGS = new MessageType("Report Tags", 3)

const chartsMap = new Map();

let ws;

try {
    console.log("Starting websocket")

    ws = new WebSocket("ws://localhost:11001");
    ws.binaryType = 'arraybuffer';

    ws.addEventListener('open', () => {
        console.log('Connected');

        console.log("Requesting beacons")
        WSProtocol.send(ws, WSProtocol.createRequestBeaconsPacket())
    });

    ws.addEventListener('error', (err) => {
        console.error('WebSocket error:', err);
    });

    ws.addEventListener('close', () => {
        console.log('Connection closed');
    });

    ws.onmessage = (event) => {

        const view = new Uint8Array(event.data)

        if (view.length < 2) {
            console.warn("Packet too small");
            return null;
        }

        const cmdId = view[0];
        const pktSize = view[1];

        switch (cmdId) {
            case WSProtocol.CMD.REGISTER_BEACON:
                break;
            case WSProtocol.CMD.REQUEST_BEACONS:
                console.log("Beacons:", WSProtocol.decodeBeaconListResponse(view));
                break;
        }


        // const dataView = new DataView(event.data);
        //
        // console.log("Received: " + dataView.getUint32());

        // const data = JSON.parse(event.data);
        //
        // const messageType = data["messageType"]
        //
        // switch (messageType) {
        //     case REPORT_ANCHORS.id:
        //         handleReportAnchors(data);
        //         break;
        //     case REPORT_ANCHOR_TELEMETRY.id:
        //         handleReportNodesData(data);
        //         break;
        //
        // }
    };

} catch (err) {
    console.error('Failed to create WebSocket:', err);
}

const points = document.querySelectorAll('.point');

points.forEach(point => {
    let offsetX, offsetY;

    point.addEventListener('mousedown', (e) => {
        offsetX = e.clientX - point.offsetLeft;
        offsetY = e.clientY - point.offsetTop;
        point.style.cursor = 'grabbing';

        const onMouseMove = (e) => {
            point.style.left = (e.clientX - offsetX) + 'px';
            point.style.top = (e.clientY - offsetY) + 'px';
        };

        const onMouseUp = () => {
            document.removeEventListener('mousemove', onMouseMove);
            document.removeEventListener('mouseup', onMouseUp);
            point.style.cursor = 'grab';
        };

        document.addEventListener('mousemove', onMouseMove);
        document.addEventListener('mouseup', onMouseUp);
    });
});

function handleReportNodesData(data) {
    const anchorData = data["anchorData"];

    const time = new Date().toLocaleTimeString();

    anchorData.forEach(anchor => {
        let chart = chartsMap.get(anchor.anchor)

        chart.data.labels.push(time)

        anchor.dataPoints.forEach(dataPoint => {
            let dataset = chart.datasetMap.get(dataPoint.tag)
            dataset.data.push(dataPoint.data)

        })

        if (chart.data.labels.length > 50) {
            chart.data.labels.shift();
            chart.data.datasets.forEach(ds => ds.data.shift());
        }

        chart.update();
    })


    // charts.forEach((chart, i) => {
    //     const val = nodeData['m' + (i + 1)] || 0;
    //     const time = new Date().toLocaleTimeString();
    //
    //     chart.data.labels.push(time);
    //     chart.data.datasets[0].data.push(val);
    //
    //     if (chart.data.labels.length > 50) {
    //         chart.data.labels.shift();
    //         chart.data.datasets[0].data.shift();
    //     }
    //
    //     chart.update();
    // });
}

function handleReportAnchors(data) {

    const anchors = data["anchors"];

    let chartView = document.getElementById("chart-view");

    clearChildElements(chartView)
    chartsMap.clear();

    anchors.forEach((anchor, i) => {

        let canvas = document.createElement("canvas");
        canvas.id = "chart" + i;
        chartView.appendChild(canvas);

        const ctx = canvas.getContext('2d');

        let datasets = []

        data["tags"].forEach(tag => {
            datasets.push({
                label: tag["name"],
                data: [],
                backgroundColor: "white",
                borderColor: tag["color"],
                fill: false
            })
        })

        let chart = new Chart(ctx, {
            type: 'line',
            data: {
                labels: ["db"],
                datasets: datasets
            },
            options: {
                animation: false,
                responsive: true,
                plugins: {
                    title: {
                        display: true,
                        text: anchor.name
                    }
                },
                layout: {
                    padding: 15
                }
            }
        });

        chart.datasetMap = new Map()
        chart.data.datasets.forEach((ds, i) => {
            chart.datasetMap.set(ds.label, chart.data.datasets[i])
        })

        chartsMap.set(anchor.name, chart)

    })
}


