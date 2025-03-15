document.addEventListener("DOMContentLoaded", () => {
  // Toggle LED function
  ledSwitch.addEventListener("change", function () {
    const state = ledSwitch.checked ? "1" : "0";

    fetch(`/toggle-led?state=${state}`)
      .then(response => response.text())
      .then(data => console.log(data))
      .catch(error => console.error("Error:", error));

    // Update switch UI
    if (ledSwitch.checked) {
      switchContainer.classList.replace("bg-gray-300", "bg-green-500");
      switchToggle.classList.add("translate-x-8");
    } else {
      switchContainer.classList.replace("bg-green-500", "bg-gray-300");
      switchToggle.classList.remove("translate-x-8");
    }
  });

  // Fetch temperature data every 2 seconds
  function updateTemperature() {
    fetch("/temperature")
      .then((response) => response.json())
      .then((data) => {
        document.getElementById('temp1').textContent = `${data.temp1 || 0}°C`;
        document.getElementById('temp2').textContent = `${data.temp2 || 0}°C`;
        document.getElementById('temp3').textContent = `${data.temp3 || 0}°C`;
      });
  }

  setInterval(updateTemperature, 2000); // Auto-update every 2 seconds
});
