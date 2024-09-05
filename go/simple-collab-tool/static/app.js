document.addEventListener('DOMContentLoaded', function() {
    const availableBlocksContainer = document.getElementById('availableBlocks');
    const dropZone = document.getElementById('dropZone');
    const generatedCode = document.getElementById('generatedCode');

    // Fetch available blocks
    fetch('/api/blocks')
        .then(response => response.json())
        .then(blocks => {
            blocks.forEach(block => {
                const blockElement = createBlockElement(block);
                availableBlocksContainer.appendChild(blockElement);
            });
            
            // Initialize dragula
            dragula([availableBlocksContainer, dropZone], {
                copy: function (el, source) {
                    return source === availableBlocksContainer;
                },
                accepts: function (el, target) {
                    return target !== availableBlocksContainer;
                }
            }).on('drop', function(el) {
                if (el.parentElement === dropZone) {
                    el.classList.add('placed');
                    generateCode();
                }
            });
        });

    function createBlockElement(block) {
        const element = document.createElement('div');
        element.className = 'block bg-blue-500 text-white p-2 mb-2 rounded cursor-move';
        element.textContent = block.label;
        element.dataset.type = block.type;
        element.dataset.id = block.id;
        return element;
    }

    function generateCode() {
        const placedBlocks = Array.from(dropZone.children);
        let code = "package main\n\nimport (\n\t\"net/http\"\n\t\"encoding/json\"\n)\n\n";

        placedBlocks.forEach((block, index) => {
            const blockType = block.dataset.type;
            const blockId = block.dataset.id;

            if (blockType === 'endpoint') {
                code += `func ${blockId}Handler(w http.ResponseWriter, r *http.Request) {\n`;
                code += `\t// TODO: Implement ${blockId.toUpperCase()} logic\n`;
                code += "}\n\n";
            }
        });

        code += "func main() {\n";
        placedBlocks.forEach((block) => {
            if (block.dataset.type === 'endpoint') {
                code += `\thttp.HandleFunc("/${block.dataset.id}", ${block.dataset.id}Handler)\n`;
            }
        });
        code += '\thttp.ListenAndServe(":8080", nil)\n';
        code += "}\n";

        generatedCode.textContent = code;
    }
});