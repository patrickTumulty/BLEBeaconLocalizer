import asyncio
import websockets
import json
import math


async def handler(websocket):
    t = 0
    while True:
        data = {f"m{i+1}": math.sin(t + i) * 50 + 50 for i in range(6)}
        await websocket.send(json.dumps(data))
        t += 0.1
        await asyncio.sleep(0.1)


async def main():
    async with websockets.serve(handler, "localhost", 9000):
        print("WebSocket server running on ws://localhost:9000")
        await asyncio.Future()  # run forever

# Run the server
asyncio.run(main())
