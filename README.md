# ThunderTrekking
Pegue aquele cone!

----------------------------------------------------------------------

## Enfim um git organizado!

Código antigo do Trekking está nos commits passados. Ver tags, que possuem
marcações importantes (competições, mudanças importantes...). Tag "Pré-organização"
possui códigos variados (visão, kinect...)

## Organização do repositório

Baseado em http://nvie.com/posts/a-successful-git-branching-model/:

master possui código sólido (leia-se: pronto para correr). Usar tags para marcar
competição usada e modificações importantes.

develop possui código em desenvolvimento. Deve ser mantido funcional. Diverge de
master e funde em master. Não usar fast-foward.

feature-* são para funções sendo feitas (quem não tiver acesso de escrita dará
pull e merge request). Diverge de develop e funde em develop. Não usar fast-foward.

release-* para código pré-competição, pré-master. Diverge de develop e funde em
develop ou master. Não usar fast-foward.

## Organização do código

include/ possui headers de código externo (não nosso).
src/ possui código fonte nosso.
scripts/ possui... scripts.

## Organização da compilação

NÃO compile na raíz. Funciona e não tem problema, mas você vai acabar esquecendo
os arquivos lá e fazer um commit com eles. O .gitignore está configurado para
ignorar pastas build/, então:
```
mkdir build
cd build
../configure
make
```
