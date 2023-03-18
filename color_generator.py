import pygame
pygame.init()

display = pygame.display.set_mode((600, 600))
clock = pygame.time.Clock()

image = pygame.image.load("colors.png")
pixels = pygame.surfarray.array3d(image)
px = 50
py = 38
counter = 0

for i in range(144):
    display.fill((0, 0, 0))
    print(f"COLOR({counter}, {counter+2}, {pixels[px, py][0]}, {pixels[px, py][1]}, {pixels[px, py][2]});")
    if (px < 210):
        px += 25
    else:
        px = 38
        py += 30
    counter += 2
    display.blit(image, (0, 0))
    pygame.draw.circle(display, (255, 0, 0), (px, py), 2)
    pygame.display.update()
    clock.tick(60)
