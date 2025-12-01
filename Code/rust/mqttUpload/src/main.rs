fn main() {
    println!("Hello, world!");
}

use rumqttc::{MqttOptions, AsyncClient, QoS, Event, Packet};
use std::time::Duration;
use std::error::Error;

#[tokio::main]

async fn main() -> Result<(), Box<dyn Error>> {

    let mut mqttoptions = MqttOptions::new("rust-beginner-client-12345", "test.mosquitto.org", 1883);
    mqttoptions.set_keep_alive(Duration::from_secs(5));

    let (client, mut eventloop) = AsyncClient::new(mqttoptions, 10);

    client.subscribe("hello/rust", QoS::AtLeastOnce).await?;
    println!("已成功订阅主题 'hello/rust'，正在等待消息...");

    loop {
        let notification = eventloop.poll().await?;

        if let Ok(Event::Incoming(Packet::Publish(p))) = notification {
            let message = String::from_utf8_lossy(&p.payload);
            println!("收到消息: {}", message);

            break;
        }
    }

    Ok(())
}
