

function showPage(id) {
    document.querySelectorAll('.page').forEach(p => p.classList.remove('active'));
    document.getElementById(id).classList.add('active');
}

const charts = [];

for (let i = 1; i <= 6; i++) {
    const ctx = document.getElementById('chart' + i).getContext('2d');
    charts.push(new Chart(ctx, {
        type: 'line',
        data: { labels: [], datasets: [{ label: 'Metric ' + i, data: [], borderColor: 'blue', fill: false }] },
        options: { animation: false, responsive: true }
    }));
}

const ws = new WebSocket("ws://localhost:9000");

ws.onmessage = (event) => {
    const data = JSON.parse(event.data);
    // Assume data = { m1: val1, m2: val2, ... m6: val6 }

    console.log("Received data")

    charts.forEach((chart, i) => {
        const val = data['m' + (i + 1)] || 0;
        const time = new Date().toLocaleTimeString();

        chart.data.labels.push(time);
        chart.data.datasets[0].data.push(val);

        if (chart.data.labels.length > 50) {
            chart.data.labels.shift();
            chart.data.datasets[0].data.shift();
        }

        chart.update();
    });
};
