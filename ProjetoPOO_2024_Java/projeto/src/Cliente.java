import java.util.ArrayList;
import java.util.InputMismatchException;
import java.util.Scanner;
import java.io.*;

public class Cliente implements Serializable {
    private String nome;
    private int nif;
    private String localizacao;


    public Cliente(String nome, int nif, String localizacao) {
        this.nome = nome;
        this.nif = nif;
        this.localizacao = localizacao;
    }

    public Cliente() {
    }


    public void editarCliente() {
        Scanner scanner = new Scanner(System.in);
        System.out.println(this);
        System.out.println("Que dados pretende editar:");

        int choice = 0;
        do {
            System.out.println("1. Nome");
            System.out.println("2. Nif");
            System.out.println("3. Localização");
            System.out.println("4. Sair");
            System.out.print("Escolha uma opção: ");
            try {
                choice = scanner.nextInt();
                scanner.nextLine();
                switch (choice) {
                    case 1:
                        String nomeAux;
                        do {
                            System.out.print("Insira o novo nome (apenas letras): ");
                            nomeAux = scanner.nextLine();
                            if (!nomeAux.matches("[a-zA-ZÀ-ÿ\\s]+")) {
                                System.out.println("Nome inválido! Deve conter apenas letras.");
                            }
                        } while (!nomeAux.matches("[a-zA-ZÀ-ÿ\\s]+"));
                        this.setNome(nomeAux);
                        System.out.println("Nome atualizado com sucesso.");
                        break;

                    case 2:
                        int nifAux;
                        do {
                            System.out.print("Insira o novo NIF (9 dígitos): ");
                            String novoNifStr = scanner.nextLine();
                            if (!novoNifStr.matches("\\d{9}")) {
                                System.out.println("NIF inválido! Deve conter exatamente 9 dígitos.");
                                continue;
                            }
                            nifAux = Integer.parseInt(novoNifStr);
                            break;
                        } while (true);
                        this.setNif(nifAux);
                        System.out.println("NIF atualizado com sucesso.");
                        break;

                    case 3:
                        String localAux;
                        do {
                            System.out.print("Insira a nova localização (Continente, Madeira ou Açores): ");
                            localAux = scanner.nextLine();
                            if (!localAux.equalsIgnoreCase("Continente") &&
                                    !localAux.equalsIgnoreCase("Madeira") &&
                                    !localAux.equalsIgnoreCase("Açores")) {
                                System.out.println("Localização inválida! Escolha entre: Continente, Madeira, Açores.");
                            }
                        } while (!localAux.equalsIgnoreCase("Continente") &&
                                !localAux.equalsIgnoreCase("Madeira") &&
                                !localAux.equalsIgnoreCase("Açores"));
                        this.setLocalizacao(localAux);
                        System.out.println("Localização atualizada com sucesso.");
                        break;

                    case 4:
                        System.out.println("Saindo da edição de cliente.");
                        break;

                    default:
                        System.out.println("Opção inválida. Tente novamente.");
                }
            } catch (InputMismatchException e) {
                System.out.println("Entrada inválida. Por favor, insira um número.");
                scanner.nextLine(); // Limpa a entrada inválida
            }
        } while (choice != 4);

    }

    public String getNome() {
        return nome;
    }

    public void setNome(String nome) {
        this.nome = nome;
    }

    public int getNif() {
        return nif;
    }

    public void setNif(int nif) {
        this.nif = nif;
    }

    public String getLocalizacao() {
        return localizacao;
    }

    public void setLocalizacao(String localizacao) {
        this.localizacao = localizacao;
    }

    @Override
    public String toString() {
        return "Cliente{" +
                "nome='" + nome + '\'' +
                ", nif=" + nif +
                ", localizacao='" + localizacao + '\'' +
                '}';
    }
}
