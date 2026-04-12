;; =============================================================================
;; CORE SETTINGS
;; =============================================================================

(setq inhibit-startup-message t
      auto-save-default nil
      make-backup-files nil)

(setq shell-file-name "/bin/bash")
(setq explicit-shell-file-name "/bin/bash")

;; Performance optimizations
(setq gc-cons-threshold 50000000)
(setq read-process-output-max (* 1024 1024))

;; Cursor and selection
(blink-cursor-mode -1)
(setq shift-select-mode nil)
(setq mark-even-if-inactive nil)
(setq pulse-flag nil)
(setq highlight-nonselected-windows nil)

;; Truncate long lines instead of wrapping
(setq-default truncate-lines t)

;; Disable UI elements
(scroll-bar-mode -1)
(tool-bar-mode -1)
(tooltip-mode -1)
(set-fringe-mode 10)
(menu-bar-mode -1)

;; Auto revert and save
(global-auto-revert-mode 1)
(auto-save-visited-mode 1)

(setq visible-bell t)

;; Line numbers
(column-number-mode)
(global-display-line-numbers-mode t)
(dolist (mode '(org-mode-hook term-mode-hook eshell-mode-hook shell-mode-hook))
  (add-hook mode (lambda () (display-line-numbers-mode 0))))

;; Indentation defaults
(setq-default indent-tabs-mode nil)
(setq-default tab-width 4)

;; Font
(set-face-attribute 'default nil :font "Iosevka" :height 130)

;; =============================================================================
;; PACKAGE SETUP
;; =============================================================================

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

;; =============================================================================
;; THEME
;; =============================================================================

(use-package doom-themes
  :config
  (setq doom-themes-enable-bold t)
  (setq doom-themes-enable-italic t)
  (load-theme 'doom-dark+ t))

(global-hl-line-mode -1)

;; Native modeline
(setq-default mode-line-format
              '("%e" " "
                (:eval (cond (buffer-read-only " RO ")
                             ((buffer-modified-p) " ** ")
                             (t "    ")))
                "%b | (%m) | L%l C%c | %p"
                (:eval (when (bound-and-true-p vc-mode)
                         (concat " | " (substring vc-mode 1))))
                " "))

(set-face-attribute 'mode-line nil :box nil :overline nil :underline nil)
(set-face-attribute 'mode-line-inactive nil :box nil :overline nil :underline nil)

;; =============================================================================
;; WHITESPACE & INDENTATION GUIDES
;; =============================================================================

(use-package whitespace
  :ensure nil
  :hook ((prog-mode . whitespace-mode)
         (conf-mode . whitespace-mode))
  :config
  (setq whitespace-style '(face tabs tab-mark spaces space-mark trailing indentation empty))
  (setq whitespace-display-mappings
        '((tab-mark 9 [187 9] [92 9])
          (space-mark 32 [183] [46])))
  (set-face-attribute 'whitespace-space nil
                      :foreground "#3a3f5a"
                      :background nil)
  (set-face-attribute 'whitespace-tab nil
                      :foreground "#4b5263"
                      :background nil)
  (set-face-attribute 'whitespace-trailing nil
                      :foreground "#1e1e1e"
                      :background "#ff6b6b")
  (set-face-attribute 'whitespace-indentation nil
                      :foreground "#4b5263"
                      :background nil)
  (set-face-attribute 'whitespace-empty nil
                      :foreground "#1e1e1e"
                      :background "#c678dd"))

(use-package highlight-indent-guides
  :hook (prog-mode . highlight-indent-guides-mode)
  :config
  (setq highlight-indent-guides-method 'character)
  (setq highlight-indent-guides-character ?\┆)
  (setq highlight-indent-guides-auto-enabled nil)
  (setq highlight-indent-guides-responsive 'top)
  (set-face-foreground 'highlight-indent-guides-character-face "#32374d")
  (set-face-foreground 'highlight-indent-guides-top-character-face "#4b5263")
  (set-face-foreground 'highlight-indent-guides-stack-character-face "#3a3f5a"))

;; =============================================================================
;; SIMPC-MODE + C++ KEYWORDS
;; =============================================================================

(use-package simpc-mode
  :vc (:url "https://github.com/rexim/simpc-mode" :rev :newest))

(add-to-list 'auto-mode-alist '("\\.cpp\\'" . simpc-mode))
(add-to-list 'auto-mode-alist '("\\.hpp\\'" . simpc-mode))

(defun my/add-cpp-keywords ()
  "Add C++ keyword highlighting to simpc-mode."
  (font-lock-add-keywords
   nil
   '(("\\<\\(class\\|public\\|private\\|protected\\|virtual\\|override\\|final\\|explicit\\|friend\\|mutable\\|inline\\|constexpr\\|consteval\\|constinit\\|concept\\|requires\\|co_await\\|co_return\\|co_yield\\)\\>" . font-lock-keyword-face)
     ("\\<\\(nullptr\\|true\\|false\\|this\\)\\>" . font-lock-constant-face)
     ("\\<\\(auto\\|decltype\\|typename\\|template\\|namespace\\|using\\|typedef\\|static_assert\\)\\>" . font-lock-keyword-face)
     ("\\<\\(static_cast\\|dynamic_cast\\|const_cast\\|reinterpret_cast\\|typeid\\|noexcept\\|alignof\\|alignas\\|sizeof\\)\\>" . font-lock-keyword-face)
     ("\\<\\(new\\|delete\\)\\>" . font-lock-keyword-face)
     ("\\<\\(try\\|catch\\|throw\\)\\>" . font-lock-keyword-face)
     ("\\<\\(std::\\)\\(vector\\|map\\|unordered_map\\|set\\|unordered_set\\|string\\|array\\|list\\|deque\\|queue\\|stack\\|pair\\|tuple\\|optional\\|variant\\|any\\|shared_ptr\\|unique_ptr\\|weak_ptr\\|function\\|thread\\|mutex\\|atomic\\|future\\|promise\\)\\>" (1 font-lock-type-face) (2 font-lock-type-face))
     ("\\<\\(std::\\)\\(cout\\|cin\\|cerr\\|endl\\|move\\|forward\\|make_shared\\|make_unique\\|make_pair\\|make_tuple\\|begin\\|end\\|size\\|empty\\|push_back\\|emplace_back\\|insert\\|erase\\|find\\|sort\\|transform\\|accumulate\\|min\\|max\\|swap\\)\\>" (1 font-lock-type-face) (2 font-lock-function-name-face))
     ("\\<\\(gl[A-Z][a-zA-Z0-9]*\\)\\>" . font-lock-function-name-face)
     ("\\<\\(glfw[A-Z][a-zA-Z0-9]*\\)\\>" . font-lock-function-name-face)
     ("\\<\\(glm::\\)\\(vec[234]\\|mat[234]\\|mat[234]x[234]\\|quat\\|perspective\\|ortho\\|lookAt\\|translate\\|rotate\\|scale\\|radians\\|degrees\\|normalize\\|cross\\|dot\\|length\\|distance\\|inverse\\|transpose\\|value_ptr\\)\\>" (1 font-lock-type-face) (2 font-lock-type-face))
     ("\\<\\(GL_[A-Z0-9_]+\\)\\>" . font-lock-constant-face)
     ("\\<\\(GLFW_[A-Z0-9_]+\\)\\>" . font-lock-constant-face))))

(add-hook 'simpc-mode-hook #'my/add-cpp-keywords)

;; =============================================================================
;; NAVIGATION (ivy + counsel + swiper)
;; =============================================================================

(use-package ivy
  :config
  (ivy-mode 1)
  (setq ivy-wrap t
        ivy-count-format "(%d/%d) "))

(use-package counsel
  :bind (("M-x" . counsel-M-x)
         ("C-x C-f" . counsel-find-file)
         ("C-x b" . counsel-switch-buffer)
         ("C-x f" . counsel-ibuffer))
  :config
  (counsel-mode 1))

(use-package swiper
  :bind (("C-s" . swiper)))

(use-package which-key
  :config
  (setq which-key-idle-delay 0.5)
  (which-key-mode))

;; =============================================================================
;; VISUAL ENHANCEMENTS & EDITING
;; =============================================================================

(use-package rainbow-delimiters
  :hook (prog-mode . rainbow-delimiters-mode))

(use-package move-text
  :bind (("M-<up>" . move-text-up)
         ("M-<down>" . move-text-down))
  :config
  (defun indent-region-advice (&rest _ignored)
    (let ((deactivate deactivate-mark))
      (if (region-active-p)
          (indent-region (region-beginning) (region-end))
        (indent-region (line-beginning-position) (line-end-position)))
      (setq deactivate-mark deactivate)))
  (advice-add 'move-text-up :after #'indent-region-advice)
  (advice-add 'move-text-down :after #'indent-region-advice))

(use-package multiple-cursors
  :bind (("C-S-<down>" . mc/mark-next-like-this)
         ("C-S-<up>" . mc/mark-previous-like-this)
         ("C-c m a" . mc/mark-all-like-this)
         ("C-c m e" . mc/edit-lines)))

(use-package iedit
  :bind (("C-c ;" . iedit-mode)))

;; =============================================================================
;; COMPILE & PROJECT COMMANDS
;; =============================================================================

(defun my/set-cpp-compile-command ()
  "Set a simple compile-and-run command for the current C++ file."
  (when (and buffer-file-name
             (member (file-name-extension buffer-file-name) '("cpp" "hpp")))
    (setq-local compile-command
                (concat
                 "g++ -Wall -o "
                 (shell-quote-argument
                  (file-name-sans-extension
                   (file-name-nondirectory buffer-file-name)))
                 " "
                 (shell-quote-argument
                  (file-name-nondirectory buffer-file-name))
                 " -lGL -lglfw -lGLU -ldl -lglad && ./"
                 (shell-quote-argument
                  (file-name-sans-extension
                   (file-name-nondirectory buffer-file-name)))))))

(add-hook 'simpc-mode-hook #'my/set-cpp-compile-command)

(setq project-switch-commands
      '((project-find-file "Find file" ?f)
        (project-compile "Compile" ?c)
        (project-shell "Shell" ?s)))

(global-set-key (kbd "<f5>") #'compile)
(global-set-key (kbd "C-c p c") #'project-compile)
(global-set-key (kbd "C-c p f") #'project-find-file)
(global-set-key (kbd "C-c p s") #'project-shell)

;; =============================================================================
;; CUSTOM KEYBINDINGS
;; =============================================================================

(defun my/smart-beginning-of-line ()
  "Move to indentation, or to start of line if already at indentation."
  (interactive)
  (let ((current-pos (point)))
    (back-to-indentation)
    (when (= current-pos (point))
      (beginning-of-line))))

(global-set-key (kbd "C-a") #'my/smart-beginning-of-line)

(global-set-key (kbd "C-;") #'kill-ring-save)
(global-set-key (kbd "C-z") #'undo)
(global-set-key (kbd "C-.") #'forward-word)
(global-set-key (kbd "C-,") #'backward-word)

;; =============================================================================
;; CUSTOM
;; =============================================================================

(custom-set-variables
 ;; custom-set-variables was added by Custom.
 ;; If you edit it by hand, you could mess it up, so be careful.
 ;; Your init file should contain only one such instance.
 ;; If there is more than one, they won't work right.
 '(package-selected-packages nil)
 '(package-vc-selected-packages '((simpc-mode :url "https://github.com/rexim/simpc-mode"))))
(custom-set-faces
 ;; custom-set-faces was added by Custom.
 ;; If you edit it by hand, you could mess it up, so be careful.
 ;; Your init file should contain only one such instance.
 ;; If there is more than one, they won't work right.
 )
