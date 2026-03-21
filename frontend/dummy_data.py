import asyncio
import websockets
import json
import math


class MessageType:
    def __init__(self, name, id):
        self.name = name
        self.id = id


class Anchor:
    def __init__(self, name, id):
        self.name = name
        self.id = id


class Tag:
    def __init__(self, name, id, color):
        self.name = name
        self.id = id
        self.color = color


REPORT_ANCHORS = MessageType("Report Anchors", 1)
REPORT_ANCHOR_TELEMETRY = MessageType("Report Anchor Telemetry", 2)
REPORT_TAGS = MessageType("Report Tags", 3)

total_nodes = 4


anchors = [
    Anchor("Alpha", 1),
    Anchor("Bravo", 2),
    Anchor("Charlie", 3),
    Anchor("Delta", 4),
]

tags = [
    Tag("Birdy", 1, "Blue"),
    Tag("Crosby", 2, "Orange"),
    Tag("Posey", 3, "Green"),
]


async def reportAnchors(ws):
    data = {
        "messageType": REPORT_ANCHORS.id,
        "anchors": [{"name": a.name, "id": a.id} for a in anchors],
        "tags": [{"name": t.name, "id": t.id, "color": t.color} for t in tags]
    }
    await ws.send(json.dumps(data))


async def handler(websocket):
    t = 0
    await reportAnchors(websocket)
    await asyncio.sleep(0.5)
    while True:
        data = {}
        data["messageType"] = REPORT_ANCHOR_TELEMETRY.id
        data["anchorData"] = []
        i = 0
        for anchor in anchors:
            i += 1
            dp = []
            for j, tag in enumerate(tags):
                dp.append({
                    "tag": tag.name,
                    "data": math.sin(t + i + (j * math.pi * 0.25)) * 50 + 50
                })

            data["anchorData"].append({
                "anchor": anchor.name,
                "dataPoints": dp
            })
        await websocket.send(json.dumps(data))
        t += 0.1
        await asyncio.sleep(0.1)


async def main():
    async with websockets.serve(handler, "localhost", 9000):
        print("WebSocket server running on ws://localhost:9000")
        await asyncio.Future()  # run forever

# Run the server
asyncio.run(main())
