import telepot
import requests
import time
import threading

TOKEN = 'YOUR_TELEGRAM_BOT_TOKEN'
bot = telepot.Bot(TOKEN)

# Replace with the actual IP address of your Nano 33 IoT
NANO_IP = '....'

# Global variable to control the data sending
send_data = False

def send_sensor_data(chat_id):
    while send_data:
        try:
            response = requests.get(f'http://{NANO_IP}/sensor-data', timeout=20)
            response.raise_for_status()  # Check for HTTP request errors
            data = response.json()
            message = (
                f"*~==============================~*\n"
                f"Temperature: {data['temperature']} °C\n"
                f"Humidity: {data['humidity']} %\n"
                f"*~===============================~*\n"
                f"SPO2: {data['spo2']} %\n"
                f"Heart Rate: {data['heartRate']} bpm\n"
                f"*~===============================~*\n"
                f"SCR: {data['scr']}\n"
                f"SCL: {data['scl']}\n"
                f"Raw: {data['raw']}\n"
                f"*~===============================~*"
            )
            bot.sendMessage(chat_id, message)
        except requests.exceptions.RequestException as e:
            bot.sendMessage(chat_id, f"Error fetching data: {e}")
        time.sleep(5)

def handle(msg):
    global send_data
    chat_id = msg['chat']['id']
    command = msg['text']

    if command == '/start':
        bot.sendMessage(chat_id, "Welcome to the sensor bot!")
    elif command == '/data':
        if not send_data:
            send_data = True
            bot.sendMessage(chat_id, "Starting to send sensor data every 5 seconds...")
            threading.Thread(target=send_sensor_data, args=(chat_id,)).start()
    elif command == '/end':
        send_data = False
        bot.sendMessage(chat_id, "Stopped sending sensor data.")
    elif command == '/ip':
        try:
            response = requests.get(f'http://{NANO_IP}/ip-address', timeout=10)
            response.raise_for_status()
            data = response.json()
            bot.sendMessage(chat_id, f"IP Address: {data['ip']}")
        except requests.exceptions.RequestException as e:
            bot.sendMessage(chat_id, f"Error fetching IP address: {e}")
    else:
        # Echo any other command or text
        bot.sendMessage(chat_id, f"Echo: {command}")

bot.message_loop(handle)

print('Listening for commands...')
while True:
    time.sleep(10)
