**Solução de Monitoramento de Saúde com ESP32 e MQTT**

Descrição do Problema de Saúde

A falta de prevenção e a acessibilidade limitada aos serviços de saúde são desafios significativos enfrentados pela sociedade contemporânea. Diagnósticos tardios e tratamentos ineficazes resultam em custos elevados e impactam diretamente na qualidade de vida das pessoas. Diante desse cenário, surge a necessidade de explorar soluções inovadoras que possam revolucionar a área da saúde, promovendo a prevenção, acessibilidade e personalização dos cuidados médicos.

Visão Geral da Solução Proposta

A solução proposta consiste em um dispositivo de monitoramento de saúde que utiliza tecnologias emergentes, como sensores IoT (Internet das Coisas), automação e análise de dados inteligente. Este dispositivo é projetado para coletar dados vitais, como temperatura corporal, umidade e frequência cardíaca, em tempo real. A comunicação é estabelecida por meio do protocolo MQTT, permitindo a transmissão eficiente dos dados para um servidor centralizado.

O dispositivo não apenas fornece uma visão abrangente da saúde do usuário, mas também atua proativamente na prevenção de doenças. A detecção precoce de padrões anômalos nos dados permite intervenções oportunas, contribuindo para um acompanhamento médico mais eficaz.

Instruções de Configuração e Execução:

Requisitos Prévios: 

->Placa de desenvolvimento ESP32 (ou simulador compatível)

->Sensores: DHT22 (temperatura e umidade)

->Componentes: Potenciômetro, botão de emergência

->Acesso à internet para comunicação MQTT


Configuração:

Instalação das Bibliotecas:
Certifique-se de ter as bibliotecas necessárias instaladas, listadas no início do código.

Configuração do WiFi:
Substitua as variáveis SSID e PASSWORD com as informações da sua rede WiFi.

Configuração do MQTT Broker:
Atualize as variáveis BROKER_MQTT e BROKER_PORT com as informações do seu broker MQTT.

Execução
Upload do Código:
Faça o upload do código para a placa ESP32 utilizando a IDE Arduino ou plataforma equivalente.

Conexão dos Componentes:
Conecte os sensores (DHT22), potenciômetro e botão de emergência aos pinos definidos no código.

Alimentação:
Alimente a placa ESP32.

Monitoramento:
Acesse o monitor serial para acompanhar a inicialização e a comunicação com o broker MQTT.

Demonstração no Wokwi:
Para uma simulação visual, utilize o ambiente Wokwi conforme apresentado no vídeo de demonstração.

Acompanhamento MQTT:
Utilize um cliente MQTT para verificar a publicação dos dados nos tópicos definidos (gm/dados/enviar e gm/dados/emergencia).

Link do Projeto: 
https://wokwi.com/projects/381494575647938561

Link do Vídeo:
https://youtu.be/AlirAhYJvMY

Integrantes:
Guilherme Dejulio Milheiro - RM550295
Marcelo Tan - RM 551771
