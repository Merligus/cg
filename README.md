# cg
Trabalho de CG

Victor Hugo Ota
Gustavo de Jesus Merli

1. Implementado em Visual Studo

2. Todas as atividades foram implementadas totalmente

3. Manual
	- Os atalhos são os mesmos implementados para o P2 com a adição do delete comentado anteriormente.
	- A opção Delete que remove um objeto de cena junto de seus filhos pode ser feito com o comando 
	Ctrl + Delete.
	- A opção de mudança de pai na janela Hierarchy é feita clicando e arrastando a célula para o
	novo pai.

4. Adições
	- No controlador da câmera foi adicionado a mudança de distância da posição da câmera
	ao ponto de foco da câmera. Ela também muda a rapidez da movimentação da câmera.
	- Opção delete.

5. Explicação matemática
	- A1, métodos para cálculo das matrizes de projeção, visão e sa inversa: 
		No método para a matriz de projeção (updateProjectionMatrix), se a opção de projeção ortográfica
	fosse selecionada, a função ortho() com seus devidos parâmetros era chamada apenas calculando
	o "width" utilizando o atributo "\_height" e "\_aspectRatio", e caso a opção de projeção perspectiva
	fosse selecionada, a função perspective() era chamada com seus devidos parâmetros. 
		No método para a matriz de visão e sua inversa (updateViewMatrix), foram utilizadas 4 variáveis
	para calcular as matrizes: 
		- WorldUp que é o vetor que aponta para cima e não precisa ser calculado já que é uma constante 
		do tipo vetor (0, 1, 0);
		- DOP (Direction Of Projection) que aponta para onde a câmera está olhando e é a terceira coluna
		da matriz 3x3 de rotação onde essa matriz é calculada a partir da conversão do atributo quatérnio
		"rotation" para uma matriz 3x3;
		- Right que aponta para a direita da câmera é calculada fazendo o produto vetorial DOP x WorldUp;
		- VUP (View Up) que aponta para cima da câmera e é calculada fazendo o produto vetorial Right x DOP.

		Além do atributo "position" as variáveis DOP e VUP foram passadas como parâmetro na função lookat()
	que calcula a matriz de visão. A sua inversa foi calculada usando a função inverse().
	- A2, setPosition(p), setEulerAngles(a), setRotation(q):
			No método setPosition(p), o versor DOP foi calculada a partir dos atributos "\_focalPoint",
		ântigo "\_position" e "\_distance" (DOP = (\_focalPoint - \position)/\_distance), em seguida a
		posição era atualizada com "p" e depois o novo ponto focal era calculado fazendo
		\_focalPoint = \_distance * DOP + \_position. 
			No método setEulerAngles(a), 

6. Objetos de cena
	- male base (Human.obj) -> https://free3d.com/3d-model/male-base-mesh-6682.html
	- iron man (IronMan.obj) -> https://free3d.com/3d-model/ironman-rigged-original-model--98611.html
	- house (House15.obj) -> https://free3d.com/3d-model/house-15-71141.html
	- bmw (BMW X5 4.obj) -> https://free3d.com/3d-model/bmw-x5-1542.html
	- resident building (Residential Buildings 0##.obj) -> https://free3d.com/3d-model/array-house-example-3033.html
	- lamborghini (Avent.obj) -> https://free3d.com/3d-model/lamborghini-aventador-42591.html
	- grass (Gras_0#.obj) -> https://free3d.com/3d-model/grass-pack-71861.html
	- tree (Tree.obj) -> https://free3d.com/3d-model/tree-74556.html
	- knight (knight.obj) -> https://free3d.com/3d-model/knight-13720.html
