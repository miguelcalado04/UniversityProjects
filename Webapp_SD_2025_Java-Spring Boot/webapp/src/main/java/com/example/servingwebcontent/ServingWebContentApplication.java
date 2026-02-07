package com.example.servingwebcontent;

import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;
import org.springframework.boot.web.servlet.ServletRegistrationBean;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.ComponentScan;

@SpringBootApplication
@ComponentScan(basePackages = "com.example.servingwebcontent")
public class ServingWebContentApplication {

	//Mesmo raciocínio: registra o servlet ThymeleafServlet na URL /thymeleafServlet/*.
    @Bean
	public ServletRegistrationBean<ThymeleafServlet> thymeleafServletBean() {
		ServletRegistrationBean<ThymeleafServlet> bean = new ServletRegistrationBean<>(new ThymeleafServlet(), "/thymeleafServlet/*");
		bean.setLoadOnStartup(1);
		return bean;
	}


	//Registra o servlet Example, que responde à URL /exampleServlet/*.
	//Ignora a anotação @WebServlet da classe Example.

	//setLoadOnStartup(1): faz o servlet ser inicializado assim que o servidor sobe (em vez de esperar o primeiro acesso).
	@Bean
	public ServletRegistrationBean<Example> exampleServletBean() {
		ServletRegistrationBean<Example> bean = new ServletRegistrationBean<>(new Example(), "/exampleServlet/*");
		bean.setLoadOnStartup(1);
		return bean;
	}
    
	//Inicia a aplicação Spring Boot.
    public static void main(String[] args) {
        SpringApplication.run(ServingWebContentApplication.class, args);
    }

}