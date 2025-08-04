*COMO CONECTAR EM VARIOS DISPOSITIVOS NA MESMA REDE LOCAL*

correr o maven assim : -> ./mvnw spring-boot:run -Dspring-boot.run.arguments=--server.address=0.0.0.0

No wsl corre este comando: 
  - hostname -I
(vai aparecer algo como : xxx.xxx.xxx.xxx aaa.aa.a.a ) o ip do wsl vai ser o : xxx.xxx.xxx.xxx

Abre a PowerShell como administrador e corre estes comandos: (no "connectaddress=" por o ip que obtivemos do hostname -I)

 - netsh interface portproxy add v4tov4 listenport=8080 listenaddress=0.0.0.0 connectport=8080 connectaddress=xxx.xxx.xxx.xxx
    (Faz o port forwarding no Windows para o WSL IP)

 - New-NetFirewallRule -DisplayName "SpringBootWSL" -Direction Inbound -LocalPort 8080 -Protocol TCP -Action Allow
    (Permite o acesso na firewall do Windows para a porta 8080)


Vai a outro dispositivo conectado à mesma internet e pesquisa:(no "xxx.xxx.xxx.xxx" por o ip que obtivemos do hostname -I)
 - http://xxx.xxx.xxx.xxx:8080

Desfruta da tua webbapp