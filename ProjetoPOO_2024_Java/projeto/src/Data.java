import java.io.Serializable;

/**
 * A classe {@code Data} representa uma data no formato dia/mês/ano.
 * Implementa a interface {@code Serializable} para permitir a serialização.
 * Também inclui métodos para validação da data e verificação de anos bissextos.
 */
public class Data implements Serializable {
    private int ano, mes, dia;

    /**
     * Construtor completo que inicializa uma instância de {@code Data}.
     *
     * @param ano o ano da data.
     * @param mes o mês da data (1 a 12).
     * @param dia o dia da data.
     */
    public Data(int ano, int mes, int dia) {
        this.ano = ano;
        this.mes = mes;
        this.dia = dia;
    }

    /**
     * Construtor padrão que cria uma instância de {@code Data} sem valores iniciais.
     */
    public Data() {
    }

    /**
     * Verifica se a data atual é válida.
     * Uma data é válida se:
     * <ul>
     *     <li>O ano é maior ou igual a 0.</li>
     *     <li>O mês está entre 1 e 12.</li>
     *     <li>O dia está dentro do intervalo permitido para o mês e ano.</li>
     * </ul>
     *
     * @return {@code true} se a data for válida; {@code false} caso contrário.
     */
    public boolean isDataValida() {
        if (ano < 0 || mes < 1 || mes > 12 || dia < 1 || dia > 31) {
            return false;
        }
        switch (mes) {
            case 4: case 6: case 9: case 11:
                return dia <= 30;
            case 2:
                if (isAnoBissexto(ano)) {
                    return dia <= 29;
                } else {
                    return dia <= 28;
                }
            default:
                return dia <= 31;
        }
    }

    /**
     * Verifica se um determinado ano é bissexto.
     * Um ano é bissexto se:
     * <ul>
     *     <li>É divisível por 4 e não por 100; ou</li>
     *     <li>É divisível por 400.</li>
     * </ul>
     *
     * @param ano o ano a ser verificado.
     * @return {@code true} se o ano for bissexto; {@code false} caso contrário.
     */
    private boolean isAnoBissexto(int ano) {
        return (ano % 4 == 0 && ano % 100 != 0) || (ano % 400 == 0);
    }

    /**
     * Retorna o ano da data.
     *
     * @return o ano da data.
     */
    public int getAno() {
        return ano;
    }

    /**
     * Define o ano da data.
     *
     * @param ano o ano a ser definido.
     */
    public void setAno(int ano) {
        this.ano = ano;
    }

    /**
     * Retorna o mês da data.
     *
     * @return o mês da data (1 a 12).
     */
    public int getMes() {
        return mes;
    }

    /**
     * Define o mês da data.
     *
     * @param mes o mês a ser definido (1 a 12).
     */
    public void setMes(int mes) {
        this.mes = mes;
    }

    /**
     * Retorna o dia da data.
     *
     * @return o dia da data.
     */
    public int getDia() {
        return dia;
    }

    /**
     * Define o dia da data.
     *
     * @param dia o dia a ser definido.
     */
    public void setDia(int dia) {
        this.dia = dia;
    }

    /**
     * Retorna uma representação textual da data no formato dia/mês/ano.
     *
     * @return uma string representando a data.
     */
    @Override
    public String toString() {
        return "Data:" +
                dia +
                "/" + mes +
                "/" + ano;
    }
}
