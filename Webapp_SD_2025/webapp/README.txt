						INFORMAÇÃO NECESSÁRIA


================ ESTRUTURA DE PASTAS ==================
projeto/ .mvn/ wrapper
maven-wrapper.jar"

projeto/ lib 
ter o jsoup-1.18.3.jar"

projeto/ src/ main/ java/ com /example/ servingwebcontent/ forms
SearchClient.java, Downloader.java, Gateway.java, StorageBarrel.java, Index.java, CallBack.java, PageInfo.java têm de estar numa pasta chamada forms 


projeto/ src/ main/ java/ com /example/ servingwebcontent/ 
GreetingController.java, HackerNewsService.java, ServingWebContentApplication.java, WebSocketConfig.java, OpenRouterService.java


projeto/ src/ main/ resources / templates
add-link.html, api.html, greeting.html, hacker-index.html, search.html, top10.html, home.html


projeto/ src/ main/ resources
aqui vai estar o application.properties

na pasta onde vamos ter o src vai ter outra pasta chamada target que vai ter uma pasta dentro chamada classes que é onde temos de ir para compilar

na pasta inicial do projeto temos que ir ao terminal (pode ser no vscode) e fazer o comando: mvn -N wrapper:wrapper para ele gerar o mvnw que vamos usar para correr o projeto



============== COMO CORRER NO TERMINAL =================

tem de ter o jsoup e o mavens instalados

no vscode ir para a pasta onde tem as pastas do src e target e o pom.xml:

executar o comando seguinte, estando na pasta principal do projeto que tem o src, target, pom.xml lá dentro
./mvn clean compile

de seguida fazemos cd target cd classes e executamos dependendo do terminal os seguintes comandos:


Correr o Downloader no terminal
mvn exec:java -Dexec.mainClass="com.example.servingwebcontent.forms.Downloader"

Depois de correr o downloader  no terminal temos que fazer cd target e cd classes para ir para a pasta classes e podermos correr os códigos abaixo

Correr o Gateway no terminal
java -cp "lib/jsoup-1.18.3.jar:." com/example/servingwebcontent/forms.Gateway 8099 8100

port1 e port2 vão ser as portas que os barrels vão usar, que vão ser impressas quando executamos os StorageBarrels e depois temos de meter como argumento no gateway 

Correr o StorageBarrel no terminal
java -cp "lib/jsoup-1.18.3.jar:." com/example/servingwebcontent/forms.StorageBarrel

ao fim de estarem todos em execução, abrimos outro terminal e colocamos o seguinte comando:
./mvnw spring-boot:run

ir a um navegador e ir à pagina : http://localhost:8081/



==================== ORDEM DE EXECUÇÃO ===================

Downloader ->  StorageBarrel -> Gateway -> ./mvnw spring-boot:run
