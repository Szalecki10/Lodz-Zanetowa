import utime
from machine import Pin, SPI
from nrf24l01 import NRF24L01

# Adresy komunikacyjne
pipes = (b'\xe1\xf0\xf0\xf0\xf0', b'\xd2\xf0\xf0\xf0\xf0')

# Konfiguracja wbudowanej diody LED
led = Pin(25, Pin.OUT)
led.value(0)

print('Pico RX Starting')

# Konfiguracja SPI i nRF24L01
spi = SPI(0, sck=Pin(18), mosi=Pin(19), miso=Pin(20))
csn = Pin(16, mode=Pin.OUT, value=1)
ce = Pin(17, mode=Pin.OUT, value=0)
nrf = NRF24L01(spi, csn, ce, channel=100, payload_size=32)

# Otwieranie rurek komunikacyjnych
nrf.open_rx_pipe(1, pipes[0])  # Adres do odbioru
nrf.start_listening()  # Rozpocznij nasłuchiwanie

print('Ready to receive packets...')

while True:
    # Odbieranie wiadomości
    if nrf.any():
        package = nrf.recv()
        try:
            # Dekodowanie liczby całkowitej
            received_int = int.from_bytes(package, 'little')
            print(f"Received integer: {received_int}")
        except ValueError:
            print(f"Error decoding integer: {package}")

    utime.sleep(0.1)