
export const WSProtocol = (() => {

    const HEADER_SIZE_BYTES = 2;

    const CMD = {
        UNKNOWN: 0,
        REGISTER_BEACON: 1,
        REQUEST_BEACONS: 2
    };

    function macToBytes(mac) {
        const clean = mac.replace(/[^0-9A-Fa-f]/g, '');
        if (clean.length !== 12) return null;

        const bytes = new Uint8Array(6);
        for (let i = 0; i < 6; i++) {
            bytes[i] = parseInt(clean.substr(i * 2, 2), 16);
        }
        return bytes;
    }

    function macBytesToString(view, offset = 0) {
        return Array.from({ length: 6 }, (_, i) =>
            view[offset + i].toString(16).padStart(2, '0')
        ).join(':');
    }

    function createRegisterBeaconPacket(macString) {
        const macBytes = macToBytes(macString);
        if (!macBytes) {
            throw new Error("Invalid MAC address");
        }

        const totalSize = HEADER_SIZE_BYTES + 6;
        const view = new Uint8Array(totalSize);

        view[0] = CMD.REGISTER_BEACON;
        view[1] = totalSize;
        view.set(macBytes, HEADER_SIZE_BYTES);

        return view.buffer;
    }

    function createRequestBeaconsPacket() {
        const view = new Uint8Array(HEADER_SIZE_BYTES);
        view[0] = CMD.REQUEST_BEACONS;
        view[1] = HEADER_SIZE_BYTES;
        return view.buffer;
    }

    function decodeBeaconListResponse(view) {

        if (view.length < HEADER_SIZE_BYTES + 1) {
            return [];
        }

        const count = view[HEADER_SIZE_BYTES];

        const macs = [];

        let offset = HEADER_SIZE_BYTES + 1;

        for (let i = 0; i < count; i++) {
            if (offset + 6 > view.length) {
                break;
            }
            macs.push(macBytesToString(view, offset));
            offset += 6;
        }
        return macs;
    }

    function send(ws, packet) {
        if (ws.readyState !== WebSocket.OPEN) {
            console.warn("WebSocket not open");
            return;
        }
        ws.send(packet);
    }

    return {
        CMD,
        createRegisterBeaconPacket,
        createRequestBeaconsPacket,
        decodeBeaconListResponse,
        send,
    };

})();
