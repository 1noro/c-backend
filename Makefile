PROJECT_NAME=html-action
GCC_VERSION=11
CURRENT_DIR := $(shell pwd)

all: help

.PHONY: help
help: ## Makefile help
	@awk 'BEGIN {FS = ":.*##"; printf "\nUsage:\n  make \033[36m<target>\033[0m\n"} /^[^{]+:.*?##/ { printf "  \033[36m%-23s\033[0m %s\n", $$1, $$2 } /^##@/ { printf "\n\033[1m%s\033[0m\n", substr($$0, 5) } ' $(MAKEFILE_LIST)
	@echo ""

.PHONY: build-cgi
build-cgi: ## Build CGI binary
#	gcc -o bin/action.cgi src/action.c
	docker run --rm -v "$(CURRENT_DIR)/src":/usr/src/myapp -w /usr/src/myapp gcc:$(GCC_VERSION) gcc -o action.cgi action.c
	@mv -f src/action.cgi bin/action.cgi

.PHONY: build-server
build-server: ## Build docker image
	docker build -t $(PROJECT_NAME)-image:latest .

.PHONY: build
build: build-cgi build-server ## Build docker image

.PHONY: up
up: build ## Build and runs the server in docker
	docker run -d --name $(PROJECT_NAME)-container -p 1337:80 -v "$(CURRENT_DIR)/public":/usr/local/apache2/htdocs/ -v "$(CURRENT_DIR)/bin":/usr/local/apache2/cgi-bin/ $(PROJECT_NAME)-image:latest
	@echo "Server running in http://localhost:1337/"

.PHONY: down
down: ## Remove the server's container
	@docker stop $(PROJECT_NAME)-container
	@docker rm $(PROJECT_NAME)-container

.PHONY: logs
logs: ## Show server's container logs
	@docker logs -f $(PROJECT_NAME)-container

.PHONY: bash
bash: ## Execs a bash shell inside the server's container
	@docker exec -it $(PROJECT_NAME)-container bash
