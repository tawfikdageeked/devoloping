;; Close the boot screen
(setq inhibit-startup-message t
      auto-save-default nil
      make-backup-files nil)

;; Fix shell for chroot
(setq shell-file-name "/bin/bash")
(setq explicit-shell-file-name "/bin/bash")

;; Make Emacs faster
(setq gc-cons-threshold 50000000)
(setq read-process-output-max (* 1024 1024))

;; Stop cursor blinking
(blink-cursor-mode -1)

;; Disable weird highlight effect
(setq pulse-flag nil)
(setq highlight-nonselected-windows nil)
(global-hl-line-mode -1)

;; Disable UI elements
(scroll-bar-mode -1)
(tool-bar-mode -1)
(tooltip-mode -1)
(set-fringe-mode 10)
(menu-bar-mode -1)

(global-auto-revert-mode 1)
(auto-save-visited-mode 1)

(setq visible-bell t)

(column-number-mode)
(global-display-line-numbers-mode t)

(dolist (mode '(org-mode-hook
                term-mode-hook
                eshell-mode-hook))
  (add-hook mode (lambda () (display-line-numbers-mode 0))))

(require 'package)
(setq package-archives '(("melpa" . "https://melpa.org/packages/")
                         ("org" . "https://orgmode.org/elpa/")
                         ("elpa" . "https://elpa.gnu.org/packages/")))
(package-initialize)
(unless package-archive-contents
  (package-refresh-contents))

(unless (package-installed-p 'use-package)
  (package-install 'use-package))
(require 'use-package)
(setq use-package-always-ensure t)

(use-package ivy
  :config
  (ivy-mode 1)
  (setq ivy-wrap t
        ivy-count-format "(%d/%d) "))

(use-package counsel
  :bind (("M-x" . counsel-M-x)
         ("C-x C-f" . counsel-find-file)
         ("C-x f" . counsel-ibuffer))
  :config
  (counsel-mode 1))

(use-package swiper
  :bind (("C-s" . swiper)))

(use-package which-key
  :config
  (which-key-mode))

(use-package doom-themes
  :config
  (setq doom-themes-enable-bold t)
  (setq doom-themes-enable-italic t)
  (load-theme 'doom-palenight t))

(with-eval-after-load 'doom-themes
  (global-hl-line-mode -1))

(use-package doom-modeline
  :init (doom-modeline-mode 1)
  :custom
  (doom-modeline-height 15))

(use-package rainbow-delimiters
  :hook (prog-mode . rainbow-delimiters-mode))

(use-package ivy-rich
  :init
  (ivy-rich-mode 1))

(use-package helpful
  :custom
  (counsel-describe-function-function #'helpful-callable)
  (counsel-describe-variable-function #'helpful-variable)
  :bind
  (([remap describe-function] . counsel-describe-function)
   ([remap describe-command] . helpful-command)
   ([remap describe-variable] . counsel-describe-variable)
   ([remap describe-key] . helpful-key)))

;; Simple autocomplete
(use-package company
  :config
  (global-company-mode 1)
  (setq company-minimum-prefix-length 2)
  (setq company-idle-delay 0.3))

;; Smart auto-close brackets
(use-package smartparens
  :config
  (require 'smartparens-config)
  (smartparens-global-mode 1)
  (show-smartparens-global-mode -1))

;; Indentation guides
(use-package highlight-indent-guides
  :hook (prog-mode . highlight-indent-guides-mode)
  :config
  (setq highlight-indent-guides-method 'character)
  (setq highlight-indent-guides-character ?\|)
  (setq highlight-indent-guides-responsive nil)
  (setq highlight-indent-guides-auto-enabled nil)
  (set-face-foreground 'highlight-indent-guides-character-face "gray50"))

;; Better C++ highlighting
(use-package modern-cpp-font-lock
  :hook (c++-mode . modern-c++-font-lock-mode))

;; Visible whitespace
(setq whitespace-style '(face spaces space-mark tabs tab-mark trailing))
(setq whitespace-display-mappings
      '((space-mark 32 [183] [46])
        (tab-mark 9 [187 9] [92 9])))
(global-whitespace-mode 1)

;; Font
(set-face-attribute 'default nil :font "Iosevka" :height 130)

;; Use spaces not tabs
(setq-default indent-tabs-mode nil)
(setq-default tab-width 4)

;; Set compile command for C++
(defun my/set-cpp-compile-command ()
  (setq compile-command
        (concat "g++ -Wall -o "
                (file-name-sans-extension (file-name-nondirectory buffer-file-name))
                " "
                (file-name-nondirectory buffer-file-name)
                " -lGL -lglfw -lGLU -ldl -lglad && ./"
                (file-name-sans-extension (file-name-nondirectory buffer-file-name)))))

(add-hook 'c++-mode-hook #'my/set-cpp-compile-command)

;; Only custom keybinding: copy
(global-set-key (kbd "C-;") #'kill-ring-save);; Close the boot screen
(setq inhibit-startup-message t
      auto-save-default nil
      make-backup-files nil)

;; Fix shell for chroot
(setq shell-file-name "/bin/bash")
(setq explicit-shell-file-name "/bin/bash")

;; Make Emacs faster
(setq gc-cons-threshold 50000000)
(setq read-process-output-max (* 1024 1024))

;; Stop cursor blinking
(blink-cursor-mode -1)
(setq shift-select-mode nil)
(setq mark-even-if-inactive nil)
(setq pulse-flag nil)
(setq highlight-nonselected-windows nil)
(global-hl-line-mode -1)

;; Disable UI elements
(scroll-bar-mode -1)
(tool-bar-mode -1)
(tooltip-mode -1)
(set-fringe-mode 10)
(menu-bar-mode -1)

(global-auto-revert-mode 1)
(auto-save-visited-mode 1)

(setq visible-bell t)

(column-number-mode)
(global-display-line-numbers-mode t)

(dolist (mode '(org-mode-hook
                term-mode-hook
                eshell-mode-hook))
  (add-hook mode (lambda () (display-line-numbers-mode 0))))

(require 'package)
(setq package-archives '(("melpa" . "https://melpa.org/packages/")
                         ("org" . "https://orgmode.org/elpa/")
                         ("elpa" . "https://elpa.gnu.org/packages/")))
(package-initialize)
(unless package-archive-contents
  (package-refresh-contents))

(unless (package-installed-p 'use-package)
  (package-install 'use-package))
(require 'use-package)
(setq use-package-always-ensure t)

(use-package ivy
  :config
  (ivy-mode 1)
  (setq ivy-wrap t
        ivy-count-format "(%d/%d) "))

(use-package counsel
  :bind (("M-x" . counsel-M-x)
         ("C-x C-f" . counsel-find-file)
         ("C-x f" . counsel-ibuffer))
  :config
  (counsel-mode 1))

(use-package swiper)

(use-package which-key
  :config
  (which-key-mode))

(use-package doom-themes
  :config
  (setq doom-themes-enable-bold t)
  (setq doom-themes-enable-italic t)
  (load-theme 'doom-palenight t))

(with-eval-after-load 'doom-themes
  (global-hl-line-mode -1))

(use-package doom-modeline
  :init (doom-modeline-mode 1)
  :custom
  (doom-modeline-height 15))

(use-package nerd-icons
  :ensure t)

(use-package rainbow-delimiters
  :hook (prog-mode . rainbow-delimiters-mode))

(use-package ivy-rich
  :init
  (ivy-rich-mode 1))

(use-package helpful
  :custom
  (counsel-describe-function-function #'helpful-callable)
  (counsel-describe-variable-function #'helpful-variable)
  :bind
  (([remap describe-function] . counsel-describe-function)
   ([remap describe-command] . helpful-command)
   ([remap describe-variable] . counsel-describe-variable)
   ([remap describe-key] . helpful-key)))

;; Simple autocomplete
(use-package company
  :config
  (global-company-mode 1)
  (setq company-minimum-prefix-length 2)
  (setq company-idle-delay 0.3))

;; Smart auto-close brackets
(use-package smartparens
  :config
  (require 'smartparens-config)
  (smartparens-global-mode 1)
  (show-smartparens-global-mode -1))

;; Indentation guides
(use-package highlight-indent-guides
  :hook (prog-mode . highlight-indent-guides-mode)
  :config
  (setq highlight-indent-guides-method 'character)
  (setq highlight-indent-guides-character ?\|)
  (setq highlight-indent-guides-responsive nil)
  (setq highlight-indent-guides-auto-enabled nil)
  (set-face-foreground 'highlight-indent-guides-character-face "gray50"))

;; Better C++ highlighting
(use-package modern-cpp-font-lock
  :hook (c++-mode . modern-c++-font-lock-mode))

;; Visible whitespace
(setq whitespace-style '(face spaces space-mark tabs tab-mark trailing))
(setq whitespace-display-mappings
      '((space-mark 32 [183] [46])
        (tab-mark 9 [187 9] [92 9])))
(global-whitespace-mode 1)

;; Font
(set-face-attribute 'default nil :font "Iosevka" :height 130)

;; Use spaces not tabs
(setq-default indent-tabs-mode nil)
(setq-default tab-width 4)

;; Set compile command for C++
(defun my/set-cpp-compile-command ()
  (setq compile-command
        (concat "g++ -Wall -o "
                (file-name-sans-extension (file-name-nondirectory buffer-file-name))
                " "
                (file-name-nondirectory buffer-file-name)
                " -lGL -lglfw -lGLU -ldl -lglad && ./"
                (file-name-sans-extension (file-name-nondirectory buffer-file-name)))))

(add-hook 'c++-mode-hook #'my/set-cpp-compile-command)
